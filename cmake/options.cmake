set(COVERAGE                         Yes    CACHE BOOL   "Generate coverage data")
set(CHECK                            No     CACHE BOOL   "add everything option")
DEFADD(AUDIO                         Yes   "Build with audio")
DEFADD(OUTPUT_WARNINGS               Yes  "Enable warning output.")
DEFADD(OUTPUT_ERRORS                 Yes  "Enable error output.")
DEFADD(OUTPUT_OUTPUT                 No   "Enable stdout  output.")
DEFADD(TESTING                       No   "Run tests.")
DEFADD(REGISTER_WINDOW               No   "Enable GB register window")
DEFADD(OUTPUT_DEBUG_FILES            No   "Output debug output files")
DEFADD(DEBUG_WINDOW                  No   "Window with current instruction and register values")
DEFADD(SPINLOCK                      No   "Use busy wait to framelimit.")
DEFADD(INTRO_ONLY                    No   "Only run the nintendo intro.")
DEFADD(DISPLAY_SHOW_FULL_FRAMEBUFFER No   "Show all of the framebuffer")
DEFADD(EMBEDDED_ROM                  No   "Use an embedded rom.")
DEFADD(EMBEDDED                      No   "embedded mode")
