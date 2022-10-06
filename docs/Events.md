# Event system for this project
Events in the system (ISRs, ...) toggle a flag in a byte. The event loop reads the new flag and executes the corresponding callback function assigned to the byte. It the clears the flag.

Instead of having an infinite loop in the main function, the infinite loop is called by starting the event loop.

# Second version of event system
The event system now employs a message queue. The event loop is idle, waiting for items to enter the queue. Event groups can be registered with the "event_register_group" function. This function returns the group number which is needed when adding messages to the queue so that the event manager knows which handler to call when it receives a message.

This way, the processor deals with events as they occur instead of polling a set of bytes for new flags. In the previous implementation, the event loop observed a set of bytes in order. When a flag was set, the event system called the associated event handler.

This method is not optimal, because if there are a lot of registered bytes (to observe), and a flag is set in a byte RIGHT after the event loop observes it, then the flag waits until the event loop comes around again to the its position. Or worse, another handler is called and increases the time before the flag is detected. This is especially important if the flag signals an mission critical event.
