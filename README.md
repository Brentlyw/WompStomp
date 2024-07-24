# WompStomp
A simple ransomware-esque program which targets common directories, and file extensions

# Disclaimer
Please, do not use this project without permission on another machine. Always run this program on a VM.
This is for educational purposes only, and you should keep it that way. I'm not responsible for what you do with this sourcecode.

# Features
- Simple XOR encryption of files within common directories (Desktop, Documents, Pictures, etc.)
- targets only certain file extensions related to documents, pictures, videos, databases, and work files.
- Hardcoded XOR key (0xAA) incase you're dumb and run this accidently, so you can reverse it.
- 500MB cap to ensure speedy encryption.

# How to compile? 
"cl /EHsc /W4 /DUNICODE /D_UNICODE WompStomp.cpp /link Shell32.lib Kernel32.lib User32.lib"
