const std = @import("std");

const flags = .{
    "-O0",
    //"-DDEBUG",
    "-fsanitize=address",
    "-Werror",
    "-g",
    "-march=native",
    "-pthread",
    "-DCPPTRACE_STATIC_DEFINE",
    "-std=c++23",
    // It's a little odd that I have to use this flag...
    "-Wno-uninitialized",
};

pub fn linkSystemLibraries(lib: *std.Build.Step.Compile) void {
    lib.linkLibCpp();
    lib.linkLibC();
    lib.linkSystemLibrary("z");
    lib.linkSystemLibrary("asan"); // fsanitize
    lib.linkSystemLibrary("zstd");
    lib.linkSystemLibrary("dl");
    lib.linkSystemLibrary("stdc++");
    lib.linkSystemLibrary("dwarf");
    lib.linkSystemLibrary("libdwarf");
}

pub const CppLib = struct {
    lib: *std.Build.Step.Compile,
    includepath: std.Build.LazyPath,
};

pub fn build_cpptrace(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) !CppLib {
    const cpptrace_dep = b.dependency("cpptrace", .{
        .target = target,
        .optimize = optimize,
    });

    const cpptrace_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
    });

    cpptrace_mod.addIncludePath(cpptrace_dep.path("include"));

    const cpptrace_lib = b.addLibrary(.{
        .linkage = .static,
        .name = "cpptrace",
        .root_module = cpptrace_mod,
    });

    // This is needed because of some checks that his cmake file does
    // It won't compile without this patch
    const patchcommand = try std.mem.join(b.allocator, "", &.{
        "patch -N ",
        cpptrace_dep.path("src/symbols/symbols_core.cpp").getPath(b),
        " ",
        b.path(".zig/symbolspatch.patch").getPath(b),
        // So that it doesn't error when the patch has already been applied
        " || true",
    });
    // Let the memory leak

    const runpatchcommand = b.addSystemCommand(&.{ "bash", "-c", patchcommand });
    cpptrace_lib.step.dependOn(&runpatchcommand.step);

    // System Dependencies
    linkSystemLibraries(cpptrace_lib);

    // Compilation
    cpptrace_lib.addIncludePath(cpptrace_dep.path("src"));
    cpptrace_lib.addCSourceFiles(.{
        .language = .cpp,
        .root = cpptrace_dep.path("src"),
        .flags = &flags,
        .files = &.{
            "binary/elf.cpp",
            "binary/mach-o.cpp",
            "binary/module_base.cpp",
            "binary/object.cpp",
            "binary/pe.cpp",
            "binary/safe_dl.cpp",
            "cpptrace.cpp",
            "ctrace.cpp",
            "demangle/demangle_with_cxxabi.cpp",
            "demangle/demangle_with_nothing.cpp",
            "demangle/demangle_with_winapi.cpp",
            "exceptions.cpp",
            "formatting.cpp",
            "from_current.cpp",
            "jit/jit_objects.cpp",
            "options.cpp",
            "platform/dbghelp_utils.cpp",
            "snippets/snippet.cpp",
            "symbols/dwarf/debug_map_resolver.cpp",
            "symbols/dwarf/dwarf_options.cpp",
            "symbols/dwarf/dwarf_resolver.cpp",
            "symbols/symbols_core.cpp",
            "symbols/symbols_with_addr2line.cpp",
            "symbols/symbols_with_dbghelp.cpp",
            "symbols/symbols_with_dl.cpp",
            "symbols/symbols_with_libbacktrace.cpp",
            "symbols/symbols_with_libdwarf.cpp",
            "symbols/symbols_with_nothing.cpp",
            "unwind/unwind_with_dbghelp.cpp",
            "unwind/unwind_with_execinfo.cpp",
            "unwind/unwind_with_libunwind.cpp",
            "unwind/unwind_with_nothing.cpp",
            "unwind/unwind_with_unwind.cpp",
            "unwind/unwind_with_winapi.cpp",
            "utils.cpp",
            "utils/error.cpp",
            "utils/io/file.cpp",
            "utils/io/memory_file_view.cpp",
            "utils/microfmt.cpp",
            "utils/string_view.cpp",
            "utils/utils.cpp",
        },
    });

    cpptrace_lib.addIncludePath(cpptrace_dep.path("include"));

    return .{
        .lib = cpptrace_lib,
        .includepath = cpptrace_dep.path("include"),
    };
}

pub fn build_libassert(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) !CppLib {
    const libassert_dep = b.dependency("libassert", .{
        .target = target,
        .optimize = optimize,
    });

    const libassert_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
    });

    const libassert_lib = b.addLibrary(.{
        .linkage = .static,
        .name = "libassert",
        .root_module = libassert_mod,
    });

    linkSystemLibraries(libassert_lib);

    const cpptrace = try build_cpptrace(b, target, optimize);
    libassert_lib.linkLibrary(cpptrace.lib);
    libassert_lib.addIncludePath(cpptrace.includepath);

    // Compilation
    libassert_lib.addIncludePath(libassert_dep.path("src"));
    libassert_lib.addIncludePath(libassert_dep.path("include"));
    libassert_lib.addCSourceFiles(.{
        .root = libassert_dep.path("src"),
        .language = .cpp,
        .flags = &flags,
        .files = &.{
            "analysis.cpp",
            "assert.cpp",
            "paths.cpp",
            "platform.cpp",
            "printing.cpp",
            "stringification.cpp",
            "tokenizer.cpp",
            "utils.cpp",
        },
    });

    return .{ .lib = libassert_lib, .includepath = libassert_dep.path("include") };
}

pub fn cereal_include_path(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) std.Build.LazyPath {
    const cereal_dep = b.dependency("cereal", .{
        .target = target,
        .optimize = optimize,
    });

    return cereal_dep.path("include");
}

pub fn build_fumo_engine(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) !*std.Build.Step.Compile {
    const fumo_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .link_libcpp = true,
    });

    const fumo_exe = b.addExecutable(.{
        .name = "fumo_engine",
        .root_module = fumo_mod,
    });

    const raylib_dep = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
    });

    linkSystemLibraries(fumo_exe);

    // Dependencies
    fumo_exe.linkLibrary(raylib_dep.artifact("raylib"));

    const cpptrace = try build_cpptrace(b, target, optimize);
    fumo_exe.linkLibrary(cpptrace.lib);
    fumo_exe.addIncludePath(cpptrace.includepath);

    const libassert = try build_libassert(b, target, optimize);
    fumo_exe.linkLibrary(libassert.lib);
    fumo_exe.addIncludePath(libassert.includepath);

    fumo_mod.addIncludePath(cereal_include_path(b, target, optimize));

    // Compilation
    fumo_exe.addIncludePath(b.path("src"));
    fumo_exe.addIncludePath(b.path("cereal"));
    fumo_exe.addCSourceFiles(.{
        .language = .cpp,
        .root = b.path("src"),
        .flags = &flags,
        .files = &.{
            "fumo_engine/collisions_and_physics/bad_unused_circle_collisions.cpp",
            "fumo_engine/collisions_and_physics/collision_runner.cpp",
            "fumo_engine/collisions_and_physics/continous_collisions.cpp",
            "fumo_engine/collisions_and_physics/gravity_field_systems.cpp",
            "fumo_engine/collisions_and_physics/gravity_update.cpp",
            "fumo_engine/collisions_and_physics/line_circle_collisions.cpp",
            "fumo_engine/collisions_and_physics/line_rect_collisions.cpp",
            "fumo_engine/collisions_and_physics/point_line_collisions.cpp",
            "fumo_engine/collisions_and_physics/rect_circle_collisions.cpp",
            "fumo_engine/core/scheduler_ecs.cpp",
            "fumo_engine/core/setup_fumo_engine.cpp",
            "fumo_engine/core/timer_systems.cpp",
            "fumo_engine/events/collided_event.cpp",
            "fumo_engine/events/dash_state_handler.cpp",
            "fumo_engine/events/event_handler_functions.cpp",
            "fumo_engine/events/event_handlers.cpp",
            "fumo_engine/events/jump_state_handler.cpp",
            "fumo_engine/events/movement_state_handler.cpp",
            "fumo_engine/events/state_handlers.cpp",
            "fumo_engine/level_editor/entity_creation.cpp",
            "fumo_engine/level_editor/hardcoded_level0.cpp",
            "fumo_engine/level_editor/parse_input.cpp",
            "fumo_engine/level_editor/screen_transition_handler.cpp",
            "fumo_engine/level_editor/selection_handling.cpp",
            "fumo_engine/serialization/fumo_serializer.cpp",
            "fumo_engine/sprite_animation_manager/animation_player.cpp",
            "fumo_engine/sprite_animation_manager/animation_renderers.cpp",
            "fumo_raylib.cpp",
            "initialization.cpp",
            "main_functions.cpp",
            "objects/factory_systems/circular_planets.cpp",
            "objects/factory_systems/planet_factory.cpp",
            "objects/factory_systems/rect_planets.cpp",
            "objects/generic_systems/utility_systems.cpp",
            "objects/player_systems/body_movement.cpp",
            "objects/player_systems/player_initializer.cpp",
            "objects/player_systems/player_input.cpp",
            "objects/rendering_systems/all_components_renderer.cpp",
            "objects/rendering_systems/draw_components.cpp",
            "register_sprites.cpp",
            "register_to_ecs.cpp",
            "main.cpp",
        },
    });

    return fumo_exe;
}

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const fumo_exe = try build_fumo_engine(b, target, optimize);

    b.installArtifact(fumo_exe);

    const run_cmd = b.addRunArtifact(fumo_exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
