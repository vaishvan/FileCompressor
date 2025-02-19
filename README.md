An file compressing and decompressing app built with cpp and Dear ImGui as the UI interface. \

Known issues
-
1. GUI is not aligned with the screen, and there is a "window within the window" which degrades the entire user experience.
2. The file system, once accessed from "Select Files" cannot be accessed again, and this forces the user to do a minimum of 2 cycles (one for compression, other for decompression)
3. Need to implement more compression algorithms.
4. Cannot access files after a certain point before the parent directory (probably due to a. restrictions put forward by the IDE or b. directory path is out of scope for the "filesystem" header in cpp)
