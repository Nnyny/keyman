
This is a proposal to rewrite  mcompile for Linux.  For this we need to  query the base keyboard data from the Linux platform, then rewriting the keyboard .kmx using the same approach as is done in mcompile for Windows, but working from the data from the x11 keyboard on Linux.

Ideally, we'd rewrite mcompile to be cross-platform (Windows, Linux, macOS), so that the keyboard interrogation would be separated from the .kmx rewriting, at least to some degree. Nevertheless it would probably be easiest to start from a standalone implementation. 
Sample program that reads US basic keyboard and compares to key value group

# Keymap

TODO check if US basic is the right Keyboard to compare with
TODO deadkeys don't work yet
TODO path for xkb/symbols as compile time option in meson
TODO check how many/which shift states we use ( at the moment we read all shiftstate-columns of US but then use only 2 colums (non-shift + shift) then use as many colums for Other )
TODO define folder to store File_US.txt" in and find better name
TODO get rid of GTK functions that are deprecated and use X11 instead
TODO retrieve name of Other keyboard and use appropriate name instead of "Other"
TODO mcompile.cpp: open mcompile -u - option - do we need that?
TODO check if I can use files from some other keyman path instead of a copy in keymap ( e.g. filesystem.h exists elsewhere)
TODO remove kbdid and kbd for Linux
TODO shiftstate-count
TODO keymap SplitToV.cpp exchange 4 with genaerated shiftcount
TODO check if part with surplus is neccessary
TODO shift-statevector
TODO Do I need HKL for Linux / can I just use a void* ??
TODO typeddef of KMX_HKL - van I delete all m_hkl from classes?
TODO define what to return when SC/VK is not found in    get_VirtualKey_Other_From_SC,...
TODO check if passed All_Vectpor as ptr/ref not as value e.g. in KMX_CharFromVK, KMX_VKUSToVKUnderlyingLayout, KMX_ImportRules
ToDo in get_VirtualKey_Other_From_SC:  what if we use  column 3(altgr) and 4 (shift+altgr) ??
TODO where to store VK_CANCEL.... in km_types.h or elsewhere?
ToDo check up to 8 shiftstates ( find symbols-file with 8)
TODO get_position_From_VirtualKey_US: take care of the other shiftstates
TODO ...

//---------------------------
TOASK is using string OK, or do we use char, wchar?
TOASK a-z, A_Z or more keys? ...
TOASK ado we need mcompile -u  option?

./mcompile -d /Projects/keyman/keyman/linux/mcompile/keymap/mcompile_test.kmx bla.dll 0407 /Projects/keyman/keyman/linux/mcompile/keymap/mcompile_test_out.kmx

./mcompile -d     /home/mcompileTest/Source/mcompile_test.kmx bla.dll 0407 /home/mcompileTest/Source_after_run_mcompile/mcompile_test_out.kmx