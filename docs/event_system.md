# Event system for this project

Events in the system (ISRs, ...) toggle a flag in a byte. The event loop reads the new flag and executes the corresponding callback function assigned to the byte. It the clears the flag.

Instead of having an infinite loop in the main function, the infinite loop is called by starting the event loop.

TODO: Add better description of event system.
