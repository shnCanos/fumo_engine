# fumo_engine
This is the game engine i made as a wrapper around raylib to add more functionality that is needed to make a scalable game.
The only goal for this engine is making my game based around gravity platformer physics, it is not intended to be used as a standalone engine, and i don't really recommend using it.
Implemented features:
- ECS
> the game engine's core. Is used to manage game data globally and provide queries for the game's systems.
> mostly follows the traditional ECS format, has packed arrays for components and systems provide the components they want to query upfront and get updated whenever an entity has a change to their components, or gets destroyd.
-  Animation/Sprites
> Based on godot's AnimationPlayer, it is a wrapper around raylib's textures to allow you to register spritesheet pngs and they will be turned into animations for sprites in the game.
- Collisions/Physics
> an okay physics library i made myself for collision detection + solving. handles capsules, lines, rectangles, circles (and also allows for rotations to the rectangles with small modifications). could be adapted to also support polygons, but it wouldn't be very optimal. most collision detection is done with naive methods, but for the purposes of this engine and the games made with it, it is good enough.
> Collision solving is done mostly with shape casting.
- Level editor
> simple GUI layout with buttons specifically related to gravity platformer, it is just used to make it easier to create the levels and place the obstacles on each screen for developing gravity platformer.
- Serialization
> relies on cereal serialization library for this functionality. allows for saving the current game progress, saving levels in the level editor, and just keeping permanent data (with backups) around to help with making the game.
- Events and Scheduler
> the game engine also provides a system for creating events that are consumed every frame. you can also add lasting events that are responsible for destroying themselves once they are done with their task/condition has been met for ending. they can also be set on a timer and so on.
> the scheduler ended up being not very useful, but it allows to add and remove systems dynamically, by setting a timer or a condition for removal

- gravity platformer
> demo game to showcase my idea for an interesting gravity based platformer. meant to showcase fumo engine's functionality and also for prototyping a possible future game idea.

Final note:
> fumo engine is just meant to lay the ground work and add enough functionality to allow me to make a proper finished 2d platformer in the future, it is not meant to be used by other people.
