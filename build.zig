const std = @import("std");

fn addSourcesFromDep(
    b: *std.Build,
    step: *std.Build.Step.Compile,
    dep: anytype,
    extension: []const u8,
    comptime include: anytype,
) !void {
    inline for (include) |prefix| {
        try addSourcesFromDepRec(b, step, prefix, dep.path(".").getPath(b), dep, extension);
    }
}

fn addSourcesFromDepRec(
    b: *std.Build,
    step: *std.Build.Step.Compile,
    current_dir: []const u8,
    dir_path: []const u8,
    dep: anytype,
    extension: []const u8,
) !void {
    const fs = std.fs;
    const allocator = b.allocator;
    const full_dir_path = try std.fs.path.join(allocator, &[_][]const u8{ dir_path, current_dir });
    defer allocator.free(full_dir_path);
    var dir = try fs.cwd().openDir(full_dir_path, .{ .iterate = true });
    defer dir.close();

    var it = dir.iterate();
    while (try it.next()) |entry| {
        const full_file_path = try std.fs.path.join(allocator, &[_][]const u8{ current_dir, entry.name });
        // This memoy cannot be freed.
        // defer allocator.free(full_file_path);

        if (entry.kind == .directory) {
            try addSourcesFromDepRec(b, step, full_file_path, dir_path, dep, extension);
        }
        if (entry.kind == .file and std.mem.endsWith(u8, entry.name, extension)) {
            std.debug.print("Adding File to compile for {s}: {s}\n", .{ step.name, full_file_path });
            step.addCSourceFile(.{ .file = dep.path(full_file_path), .language = .cpp, .flags = &.{"-std=c++23"} });
        }
    }
}

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
    cpptrace_lib.addIncludePath(cpptrace_dep.path("include"));
    try addSourcesFromDep(b, cpptrace_lib, cpptrace_dep, ".cpp", .{"src"});

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
    try addSourcesFromDep(b, libassert_lib, libassert_dep, ".cpp", .{"src"});

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
    try addSourcesFromDep(b, fumo_exe, b, ".cpp", .{"src"});

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
