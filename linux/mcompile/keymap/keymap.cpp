#include "keymap.h"

#include <xkbcommon/xkbcommon.h>

int write_US_ToVector( v_dw_3D &vec,std::string language, const char* text) {

  std::string FullPathName = "/usr/share/X11/xkb/symbols/" + language;

  const char* path = FullPathName.c_str();
  FILE* fp = fopen((path), "r");
  if ( !fp) {
    wprintf(L"ERROR: could not open file!\n");
    return 1;
  }

  // create 1D-vector of the complete line
  v_str_1D Vector_completeUS;
  if( createCompleteRow_US(Vector_completeUS,fp , text, language)) {
    wprintf(L"ERROR: can't Create complete row US \n");
    return 1;
  }

  // split contents of 1D Vector to 3D vector
  if( split_US_To_3D_Vector( vec,Vector_completeUS)) {
    return 1;
  }
  wprintf(L"\n   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  wprintf(L"   +++++++ dimensions of Vector after split_US_To_3D_Vector (languages..characters..shiftstates)\t %li..%li..%li\n", vec.size(), vec[0].size(),vec[0][0].size());

  fclose(fp);
  return 0;
}

bool  createCompleteRow_US(v_str_1D &complete_List, FILE* fp, const char* text, std::string language) {
  // in the Configuration file we find the appopriate paragraph between "xkb_symbol <text>" and the next xkb_symbol
  // and then copy all rows starting with "key <" to a 1D-Vector

  int buffer_size = 512;
  char buffer[buffer_size];
  bool print_OK   = false;
  const char* key = "key <";
  std::string str_txt(text);
  std::string xbk_mark = "xkb_symbol";
  // _S2 TODO define folder to store File in
  std::ofstream KeyboardFile("File_" + language + ".txt");

  KeyboardFile << "Keyboard" << text << "\n";

  if (fp) {
    while (fgets(buffer, buffer_size, fp) != NULL) {
      std::string str_buf(buffer);

      // stop when finding the mark xkb_symbol
      if (std::string(str_buf).find(xbk_mark) != std::string::npos)
        print_OK = false;

      // start when finding the mark xkb_symbol + correct layout
      if ((std::string(str_buf).find(str_txt) != std::string::npos))
        print_OK = true;

      // as long as we are in the same xkb_symbol layout block and find "key <" we push the whole line into a 1D-vector
      if ((print_OK) && (std::string(str_buf).find(key) != std::string::npos)) {
        complete_List.push_back(buffer);
        KeyboardFile << buffer;
      }
    }
  }
  complete_List.push_back("    key <SPCE>  { [ space,        space] };");
  //complete_List.push_back("    key <AC12>  { [ backslash,    bar  ] };");

  if (complete_List.size() <1) {
    wprintf(L"ERROR: can't create row from US \n");
    return 1;
  }
  return 0;
}

KMX_DWORD convertNamesToValue(std::wstring tok_wstr){
  std::map<std::wstring, KMX_DWORD > first;

  first[L"exclam"]           =  33;
  first[L"at"]               =  64;
  first[L"numbersign"]       =  35;
  first[L"dollar"]           =  36;
  first[L"percent"]          =  37;
  first[L"dead_circumflex"]  =  94;
  first[L"ampersand"]        =  38;
  first[L"asterisk"]         =  42;
  first[L"parenleft"]        =  40;
  first[L"parenright"]       =  41;

  first[L"minus"]            =  45;
  first[L"underscore"]       =  95;
  first[L"equal"]            =  61;
  first[L"plus"]             =  43;
  first[L"bracketleft"]      =  91;
  first[L"braceleft"]        = 123;
  first[L"bracketright"]     =  93;
  first[L"braceright"]       = 125;
  first[L"semicolon"]        =  59;
  first[L"colon"]            =  58;
  first[L"apostrophe"]       =  39;
  first[L"quotedbl"]         =  34;
  first[L"backslash"]        =  92;
  first[L"bar"]              = 124;
  first[L"comma"]            =  44;
  first[L"less"]             =  60;
  first[L"period"]           =  46;
  first[L"greater"]          =  62;
  first[L"slash"]            =  47;
  first[L"question"]         =  63;
  first[L"space"]            =  32;
  first[L"asciitilde"]       = 126;
  first[L"asciicircum"]      = 136;

  first[L"dead_acute"]     =   180;
  first[L"grave"]          =    96;
  first[L"ssharp"]         =   223;
  //first[L" ??   "]           =   VK_OEM_102;    /* DE =  226 ' " ? VK_OEM_102 */

  if ( tok_wstr.size() == 1) {
    return (KMX_DWORD) ( *tok_wstr.c_str() );;
  }
  else {
		std::map<std::wstring, KMX_DWORD > ::iterator it;
		for (it = first.begin(); it != first.end(); ++it) {
			if (it->first == tok_wstr)
				return it->second;
		}
  }
  return returnIfCharInvalid;
}

int split_US_To_3D_Vector(v_dw_3D &all_US,v_str_1D completeList) {
  // 1: take the whole line of the 1D-Vector and remove unwanted characters.
  // 2: seperate the name e.g. key<AD06> from the shiftstates
  // 3: convert to KMX_DWORD
  // 4: push Names/Shiftstates to shift_states and then shift_states to All_US, our 3D-Vector holding all Elements

  std::vector<char> delim{' ', '[', ']', '}', ';', '\t', '\n'};
  char split_bracel = '{';
  char split_char_komma  = ',';
  int Keycde;
  v_str_1D tokens;
  v_dw_1D tokens_dw;
  v_dw_2D shift_states;
  KMX_DWORD tokens_int;
  std::wstring tok_wstr;

  // loop through the whole vector
  for (int k = 0; k < (int)completeList.size() ; k++) {

    // remove all unwanted char
    for (int i = 0; i < (int) delim.size(); i++) {
      completeList[k].erase(remove(completeList[k].begin(), completeList[k].end(), delim[i]), completeList[k].end());
    }

    // only lines with ("key<.. are of interest
    if (completeList[k].find("key<") != std::string::npos) {

      //split off the key names
      std::istringstream split1(completeList[k]);
      for (std::string each; std::getline(split1, each, split_bracel); tokens.push_back(each));

      // replace keys names with Keycode (<AD06> with 21,...)
      Keycde = replace_PosKey_with_Keycode_use_Lin(tokens[0]);
      tokens[0] = std::to_string(Keycde);

      // seperate rest of the vector to its elements and push to 'tokens'
      std::istringstream split(tokens[1]);
      tokens.pop_back();

      for (std::string each; std::getline(split, each, split_char_komma); tokens.push_back(each));

      // now convert all to KMX_DWORD and fill tokens
      tokens_dw.push_back((KMX_DWORD) Keycde);

      for ( int i = 1; i< (int) tokens.size();i++) {

        // replace a name with a single character ( a -> a  ; equal -> = ) 
        tokens_int = convertNamesToValue( wstring_from_string(tokens[i]));
        tokens_dw.push_back(tokens_int);
      }

      //wprintf(L"  Keyval  %i:   %i (%c) --- %i (%c)  \n", tokens_dw[0],tokens_dw[1],tokens_dw[1], tokens_dw[2], tokens_dw[2]);
   
      // now push result to shift_states
      shift_states.push_back(tokens_dw);
      tokens_dw.clear();
      tokens.clear();
    }
  }
  all_US.push_back(shift_states);

  if ( all_US.size() == 0) {
    wprintf(L"ERROR: Can't split US to 3D-Vector\n");
    return 1;
  }
  return 0;
}

int replace_PosKey_with_Keycode_use_Lin(std::string  in) {
  int out = returnIfCharInvalid;

// _S2 these are the Scancode-Values we use in Keyman ( = like the windows scancodes)
  //     NAME IN SYMBOLS-FILE      KEYCODE (LIN STYLE)      (WIN STYLE)       VK_US      VK_DE
                                             /*on US keyb;*/
  if      ( in == "key<TLDE>")    out = 49;                /*                VK_  */  // TOASK correct ???
  else if ( in == "key<AE01>")    out = 10;                /* 0X02           VK_1 */
  else if ( in == "key<AE02>")    out = 11;                /* 0X03           VK_2  */
  else if ( in == "key<AE03>")    out = 12;                /* 0X04           VK_3  */
  else if ( in == "key<AE04>")    out = 13;                /* 0X05           VK_4  */
  else if ( in == "key<AE05>")    out = 14;                /* 0X06           VK_5  */
  else if ( in == "key<AE06>")    out = 15;                /* 0X07           VK_6  */
  else if ( in == "key<AE07>")    out = 16;                /* 0X08           VK_7  */
  else if ( in == "key<AE08>")    out = 17;                /* 0X09           VK_8  */
  else if ( in == "key<AE09>")    out = 18;                /* 0X0A           VK_9  */
  else if ( in == "key<AE10>")    out = 19;                /* 0X0B           VK_0  */
  else if ( in == "key<AE11>")    out = 20; /*out = 61;*/  /* 0X0C           VK_MINUS   de ẞ*/
  else if ( in == "key<AE12>")    out = 21;                /* 0X0D           VK_EQUALS  DE ' */

  else if ( in == "key<AD01>")    out = 24;                /* 0X10            VK_Q  */
  else if ( in == "key<AD02>")    out = 25;                /* 0X11            VK_W  */
  else if ( in == "key<AD03>")    out = 26;                /* 0X12            VK_E  */
  else if ( in == "key<AD04>")    out = 27;                /* 0X13            VK_R  */
  else if ( in == "key<AD05>")    out = 28;                /* 0X14            VK_T  */
  else if ( in == "key<AD06>")    out = 29; /*out = 52;*/  /* 0X15            VK_Y  */
  else if ( in == "key<AD07>")    out = 30;                /* 0X16            VK_U  */
  else if ( in == "key<AD08>")    out = 31;                /* 0X17            VK_I  */
  else if ( in == "key<AD09>")    out = 32;                /* 0X18            VK_O  */
  else if ( in == "key<AD10>")    out = 33;                /* 0X19            VK_P  */
  else if ( in == "key<AD11>")    out = 34; /*out = 17;*/  /* 0X1A            VK_LEFTBRACE   DE Ü */
  else if ( in == "key<AD12>")    out = 35; /*out = 18;*/  /* 0X1B            VK_RIGHTBRACE  DE + */

  else if ( in == "key<AC01>")    out = 38;                /* 0X1E            VK_A  */
  else if ( in == "key<AC02>")    out = 39;                /* 0X1F            VK_S  */
  else if ( in == "key<AC03>")    out = 40;                /* 0X20            VK_D  */
  else if ( in == "key<AC04>")    out = 41;                /* 0X21            VK_F  */
  else if ( in == "key<AC05>")    out = 42;                /* 0X22            VK_G  */
  else if ( in == "key<AC06>")    out = 43;                /* 0X23            VK_H  */
  else if ( in == "key<AC07>")    out = 44;                /* 0X24            VK_J  */
  else if ( in == "key<AC08>")    out = 45;                /* 0X25            VK_K  */
  else if ( in == "key<AC09>")    out = 46;                /* 0X26            VK_L  */
  else if ( in == "key<AC10>")    out = 47; /*out = 59;*/  /* 0X27            VK_SEMICOLON  DE Ö*/
  else if ( in == "key<AC11>")    out = 48; /*out = 51;*/  /* 0X28            VK_APOSTROPHE DE Ä */
  //else if ( in == "key<AC12>")    out = 51; /*out = 20;*/  /* 0X29            VK_GRAVE  DE # */

  else if ( in == "key<AB01>")    out = 52; /*out = 29;*/  /* 0X2C            VK_Z  */
  else if ( in == "key<AB02>")    out = 53;                /* 0X2D            VK_X  */
  else if ( in == "key<AB03>")    out = 54;                /* 0X2E            VK_C  */
  else if ( in == "key<AB04>")    out = 55;                /* 0X2F            VK_V  */
  else if ( in == "key<AB05>")    out = 56;                /* 0X30            VK_B  */
  else if ( in == "key<AB06>")    out = 57;                /* 0X31            VK_N  */
  else if ( in == "key<AB07>")    out = 58;                /* 0X32            VK_M  */
  else if ( in == "key<AB08>")    out = 59;                /* 0X33            VK_ COMMA */
  else if ( in == "key<AB09>")    out = 60;                /* 0X34            VK_DOT  */
  else if ( in == "key<AB10>")    out = 61; /*out = 16;*/  /* 0X35            VK_SLASH  DE - */
  else if ( in == "key<BKSL>")    out = 51;                /* 0X29            VK_BKSLASH  */
  else if ( in == "key<LSGT>")    out = 63;                /* 0X37            VK_RIGHTSHIFT  */
  else if ( in == "key<SPCE>")    out = 65;                /* 0X20 ?? 39?     VK_SPACE  */

  return out;
}

v_dw_2D create_empty_2D( int dim_rows,int dim_shifts) {

  v_dw_1D shifts;
  v_dw_2D Vector_2D;

  for ( int i=0; i< dim_rows;i++) {
    for ( int j=0; j< dim_shifts;j++) {
      shifts.push_back(returnIfCharInvalid);
    }
    Vector_2D.push_back(shifts);
    shifts.clear();
  }
  return Vector_2D;
}

int append_other_ToVector(v_dw_3D &All_Vector,GdkKeymap * keymap) {

  // create a 2D vector all filled with " " and push to 3D-Vector
  v_dw_2D Other_Vector2D = create_empty_2D(All_Vector[0].size(),All_Vector[0][0].size());

  if (Other_Vector2D.size() == 0) {
    wprintf(L"ERROR: can't create empty 2D-Vector\n");
    return 1;
  }
  All_Vector.push_back(Other_Vector2D);
  wprintf(L"   +++++++ dimensions of Vector after append_other_ToVector\t\t\t\t\t\t %li..%li..%li\n", All_Vector.size(), All_Vector[0].size(),All_Vector[0][0].size());
  wprintf(L"   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

  if (All_Vector.size() < 2) {
    wprintf(L"ERROR: creation of 3D-Vector failed\n");
    return 1;
  }

  for(int i =0; i< (int) All_Vector[1].size();i++) {

    // get key name US stored in [0][i][0] and copy to name in "other"-block[1][i][0]
    All_Vector[1][i][0] = All_Vector[0][i][0];

    // get Keyvals of this key and copy to unshifted/shifted in "other"-block[1][i][1] / block[1][i][2]
    All_Vector[1][i][0+1] = getKeyvalsFromKeymap(keymap,All_Vector[0][i][0],0);   //shift state: unshifted:0
    All_Vector[1][i][1+1] = getKeyvalsFromKeymap(keymap,All_Vector[0][i][0],1);   //shift state: shifted:1

    //wprintf(L" Keycodes US dw        :   %d (US): -- %i (%c)  -- %i (%c) ---- (other): %i (%c)  --  %i(%c)    \n",(All_Vector[1][i][0]),All_Vector[0][i][1],All_Vector[0][i][1],All_Vector[0][i][2],All_Vector[0][i][2],All_Vector[1][i][1] ,All_Vector[1][i][1],All_Vector[1][i][2],All_Vector[1][i][2]);
    //wprintf(L"   Keycodes ->Other dw:-:   %d (US): -- %i (%c)  -- %i (%c)   \n\n",(All_Vector[1][i][0]),All_Vector[1][i][1],All_Vector[1][i][1],All_Vector[1][i][2],All_Vector[1][i][2]);
  }
  return 0;
}

KMX_DWORD getKeyvalsFromKeymap(GdkKeymap *keymap, guint keycode, int shift_state_pos) {
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;
  KMX_DWORD out;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return 0;
  //if(!gdk_wayland_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
  //  return 0;    https://codebrowser.dev/gtk/gtk/gdk/wayland/gdkkeys-wayland.c.html

  if (!(shift_state_pos <= count))
    return 0;

  out =(KMX_DWORD)  keyvals[shift_state_pos];

  // _S2 if out of range of what ( ascii??) return 0 or other value ?
  if (out > 255) {
    wprintf(L"out of range: found value out( %i) for keycode = %i /shift_state_pos %i      (49= TLDE 21= VK_EQUALS on US keyboard) \n", out,keycode,shift_state_pos);
    //out = 0;
  }

  g_free(keyvals);
  g_free(maps);

  return out;
}

KMX_DWORD  mapVK_To_char(KMX_DWORD SC ){
  // if there is a Keyman VK.. defined map to Keyman VKcode

 // if ( SC == 49)   return   VK_BKSLASH;     /* ; 220          = ` oder ^ */
 // if ( SC == 20)   return   VK_LBRKT;       /* ; 219          = - oder ß */
 // if ( SC == 21)   return   VK_RBRKT;       /* ; 221          = = oder ' */

 // if ( SC == 34)   return   VK_COLON;       /* ; 186 VK_OEM_4 = [ oder ü */
  //if ( SC == 35)   return   VK_EQUAL;       /* ; 187          = ] oder + */

 // if ( SC == 47)   return   VK_ACCENT;      /* ; 192 VK_OEM_1 = : oder ö */
 // if ( SC == 48)   return   VK_QUOTE;       /* ' 222 VK_OEM_7 = " oder Ä */
 // if ( SC == 51)   return   VK_SLASH;       /* ; 191          = \ oder # */

 // if ( SC == 59)   return   VK_COMMA;       /* ; 188          = , oder , */
 // if ( SC == 60)   return   VK_PERIOD;      /* ; 190          = . oder . */
 // if ( SC == 61)   return   VK_HYPHEN;      /* ; 189          = / oder - */

 // if ( SC == 65)   return   VK_SPACE;       /* ;  32 VK_SPACE =   oder   */
 // else
    return SC;
}
// _S2 TODO is this correct ??
KMX_DWORD  mapChar_To_VK(KMX_DWORD chr ){
  // if there is a Keyman VK.. defined map to Keyman VKcode

 // if ( SC == 49)   return   VK_BKSLASH;     /* ; 220          = ` oder ^ */
 // if ( SC == 20)   return   VK_LBRKT;       /* ; 219          = - oder ß */
 // if ( SC == 21)   return   VK_RBRKT;       /* ; 221          = = oder ' */

 // if ( chr == VK_COLON)   return   220;       /* ; 186 VK_OEM_4 = [ oder ü */
//  if ( chr == 187)        return   42;       /* ; 186 VK_OEM_4 = [ oder ü */
 // if ( SC == 35)   return   VK_EQUAL;       /* ; 187          = ] oder + */

 // if ( SC == 47)   return   VK_ACCENT;      /* ; 192 VK_OEM_1 = : oder ö */
 // if ( SC == 48)   return   VK_QUOTE;       /* ' 222 VK_OEM_7 = " oder Ä */
 // if ( SC == 51)   return   VK_SLASH;       /* ; 191          = \ oder # */

 // if ( SC == 59)   return   VK_COMMA;       /* ; 188          = , oder , */
 // if ( SC == 60)   return   VK_PERIOD;      /* ; 190          = . oder . */
 // if ( SC == 61)   return   VK_HYPHEN;      /* ; 189          = / oder - */

 // if ( SC == 65)   return   VK_SPACE;       /* ;  32 VK_SPACE =   oder   */
 // else
    return chr;
}
KMX_DWORD get_VirtualKey_Other_GDK( GdkKeymap *keymap, KMX_DWORD keycode) {

  GdkModifierType consumed;
  GdkKeymapKey *maps;
  guint *keyvals;
  guint lowerCase;
  guint upperCase;
  gint count;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return 0;

  //Shift
    //GdkModifierType MOD_Shift = (GdkModifierType) (  ~consumed & GDK_SHIFT_MASK );
    GdkModifierType MOD_Shift = (GdkModifierType) ( GDK_SHIFT_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_Shift , 0, keyvals, NULL, NULL, & consumed);

    for (int i = 0; i < count; i++) {
      if (maps[i].level > 1 || maps[i].group > 1)
        continue;

      gchar * kv_name =  gdk_keyval_name (keyvals[i]);

      if ( keyvals[i]>0)
        gdk_keyval_convert_case (*kv_name, &lowerCase, &upperCase);

      // _S2 is ( lowerCase == upperCase )  true for all number keys for all keyboards?
      if ( lowerCase == upperCase )
        return  (KMX_DWORD)  upperCase;
    }


  // _S2 ToDo
  // either it gives the correct rgkeys (all non-char filled with special char) or
  // it gives not all rgkeys but nr, a-z are filled correctly
    // _S2 ToDo tidy up
    if ( keycode >7) {
      UINT VK_for_rgKey2 = ScanCodeToUSVirtualKey[keycode-8];

    //return  (KMX_DWORD) *keyvals;}  //_S2 what to return if >255
    return  (KMX_DWORD) VK_for_rgKey2 ; }

  return 0;   //_S2 what to return if not found
}

// _S2 not needed, can go later
// return RETURN NON SHIFTED CHAR [1]  the VirtualKey of the US Keyboard for given Scancode
KMX_DWORD get_VirtualKey_US_From_SC(KMX_DWORD SC , v_dw_3D &All_Vector){
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[0].size()-1;i++) {
    if  ( All_Vector[0][i][0] == SC ) {
      return All_Vector[0][i][1] ;
    }
  }
  return 0;    //_S2 TODO what do I return if not found??
}

// return the Scancode of for given VirtualKey of Other Keyboard
KMX_DWORD get_SC_From_VirtualKey_Other(KMX_DWORD VK_Other , v_dw_3D &All_Vector){
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[1].size()-1;i++) {
    if  ( All_Vector[1][i][1] == VK_Other ) {
      return All_Vector[1][i][0] ;
    }
  }
  return 0;    //_S2 TODO what do I return if not found??
}

// return the Scancode of for given VirtualKey of Other US
KMX_DWORD get_SC_From_VirtualKey_US(KMX_DWORD VK_US , v_dw_3D &All_Vector){
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[0].size()-1;i++) {
    if  ( All_Vector[0][i][2] == VK_US ) {
      return All_Vector[0][i][0] ;
    }
  }
  return 0;    //_S2 TODO what do I return if not found??
}

// returns the position in All_Vector where VK_Other is found
KMX_DWORD get_position_From_VirtualKey_Other(KMX_DWORD VK_Other , v_dw_3D &All_Vector, int which_columns) {
  // find correct row of char in US
  if((which_columns <0  ) )
    return 0;

  // search all columns
  if(which_columns >(int)All_Vector[1][0].size()) {
    for( int i=1; i< (int)All_Vector[1][0].size();i++) {
      for( int j=0; j< (int)All_Vector[1].size()-1;j++) {
      if ( ( All_Vector[1][j][i] == VK_Other ) )
        return j;
      }
    }
  }

  else {
    for( int j=0; j< (int)All_Vector[1].size()-1;j++) {
        if ( ( All_Vector[1][j][which_columns] == VK_Other ) )
        return j;
      }
  }

  return 0;    //_S2 TODO what do I return if not found??
}

// returns KeyCode which hold the Keysym/Keyval (in unshifted, shifted)
KMX_DWORD get_KeyCode_From_KeyVal_GDK(GdkKeymap *keymap, UINT Keyval ) {
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  for (int k=0; k<255 ; k++ ){
    if (gdk_keymap_get_entries_for_keycode (keymap, k, &maps, &keyvals, &count)) {
      if ( (keyvals[0] == Keyval) || (keyvals[1] == Keyval) ) {
        return (KMX_DWORD) maps[0].keycode;
      }
    }
  }
  return 0;    //_S2 TODO what do I return if not found??
}
// returns KeyCode which holds the Keysym (in unshifted, shifted)
/*KMX_DWORD get_SC_From_Keycode_GDK(GdkKeymap *keymap, UINT SC ) {
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  for (int k=0; k<255 ; k++ ){
    if (gdk_keymap_get_entries_for_keycode (keymap, k, &maps, &keyvals, &count)) {
      if ( (keyvals[0] == SC) || (keyvals[1] == SC) ) {
        return (KMX_DWORD) maps[0].keycode;
      }
    }
  }
  return 0;    //_S2 TODO what do I return if not found??
}*/

// _S2 TODO How to do mapping between Linux keycodes and keyman SC
const int Lin_KM__map(int i, v_dw_3D &All_Vector) {
  // MAP:
  // VK KEYMAN-STYLE  ->  KEYCODE LINUX-STYLE
  // e.g 188 -> 59
  //All_Vector_[ 1 ][ in which line of US did find the value 58 ][ take second or third column wherever I find 58 ]]
  // finds  59th row (not value 59)
  int dw=0;
  //if (i == 32  ) return   ; /*        */5
      //if (i == 186 ) return 220;  /* Ü      */
      //if (i == 187 ) return  42;  /* + *    */
      //if (i == 188 )          {wprintf(L" swapped:  i (%i) to 59  \n",dw,i);       return  59;  }/* COMMA  */
      //if (i == 189 )          {wprintf(L" swapped:  i (%i) to 95  \n",dw,i);       return  95;  }/*   - _  */
      //if (i == 190 )          {wprintf(L" swapped:  i (%i) to 58  \n",dw,i);       return  58;  }/* PERIOD */
      //if (i == 191 )          {wprintf(L" swapped:  i (%i) to 35  \n",dw,i);       return  35;   }/* #  '   */
      //if (i == 191 )          {wprintf(L" swapped:  i (%i) to 63  \n",dw,i);       return  63; }/*       */
      //if (i == 214 )          {wprintf(L" swapped:  i (%i) to 192  \n",dw,i);       return 192;  }/*  Ö     */
      //if (i == 219 )          {wprintf(L" swapped:  i (%i) to 223  \n",dw,i);       return 223;  }/*  Sharp-S+  ?  */
      //if (i == 220 )          {wprintf(L" swapped:  i (%i) to 92  \n",dw,i);       return  92;  }/*  ^ °   */
      //if (i == 221 )          {wprintf(L" swapped:  i (%i) to 180  \n",dw,i);       return 180;  }/*  ' `   */
      //if (i == 223 )          {wprintf(L" swapped:  i (%i) to 59  \n",dw,i);       return    ; }/*       */

      //if (i == 226 )          {wprintf(L" swapped:  i (%i) to 60  \n",dw,i);       return  60;  }/*  < >   */
      //if (i == 65105 )        {wprintf(L" swapped:  i (%i) to 92  \n",dw,i);       return  92; }/*    */

      //  e.g. rgKey[192]  contains character 214
      //if (i == 192 )          {wprintf(L" swapped:  i (%i) to 214  \n",dw,i);       return 214;  }/* Ö      */
      //if (i == 186 )          {wprintf(L" swapped:  i (%i) to 220  \n",dw,i);       return 220;  }/* Ü      */
      //if (i == 222 )          {wprintf(L" swapped:  i (%i) to 196  \n",dw,i);       return 196;  }/* Ä      */
      //if (i == 220)             {wprintf(L" swapped:  i (%i) to 196  \n",dw,i);       return 186;  }/* Ä      */
      //if (i == 42)              {wprintf(L" swapped:  i (%i) to 196  \n",dw,i);       return 187;  }/* +      */

  return i;
}

// _S2 TODO
std::wstring  get_KeyVals_according_to_Shiftstate(GdkKeymap *keymap, guint keycode, ShiftState ss, int caps  ){

  GdkModifierType consumed;
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  // _S2 TODO what to return if it fails?
  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return L"1";

  //unshifted
  if (( ss == Base ) && ( caps == 0 )) {
    GdkModifierType MOD_base = (GdkModifierType) ( ~GDK_MODIFIER_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_base , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  //SHIFT+CAPS
  else if ( ( ss == Shft ) && ( caps ==1 )) {
    GdkModifierType MOD_ShiftCaps= (GdkModifierType) ((GDK_SHIFT_MASK | GDK_LOCK_MASK));
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_ShiftCaps , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  //Shift
  else if (( ss == Shft ) && ( caps == 0 )) {
    return std::wstring(1, (int) get_VirtualKey_Other_GDK(keymap,  keycode));
  }

  //caps
  else if (( ss == Base ) && ( caps == 1 )) {
    GdkModifierType MOD_Caps = (GdkModifierType) ( GDK_LOCK_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_Caps, 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  /*//ALT-GR
  else if {
    GdkModifierType MOD_AltGr = (GdkModifierType) ( GDK_MOD5_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_AltGr , 0, keyvals, NULL, NULL, & consumed);
    return *keyvals;
  }*/

  else
    return L"0";
}


std::wstring  get_KeySyms_according_to_Shiftstate(GdkKeymap *keymap, guint keycode, ShiftState ss, int caps  ){

  GdkModifierType consumed;
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return L"\0";
    //return L"1";

  //unshifted
  if (( ss == Base ) && ( caps == 0 )) {
    GdkModifierType MOD_base = (GdkModifierType) ( ~GDK_MODIFIER_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_base , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  //SHIFT+CAPS
  else if ( ( ss == Shft ) && ( caps ==1 )) {
    GdkModifierType MOD_ShiftCaps= (GdkModifierType) ((GDK_SHIFT_MASK | GDK_LOCK_MASK));
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_ShiftCaps , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  //Shift
  else if (( ss == Shft ) && ( caps == 0 )) {
    GdkModifierType MOD_Shift = (GdkModifierType) ( GDK_SHIFT_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_Shift , 0, keyvals, NULL, NULL, & consumed);
    std::wstring rV1= std::wstring(1, (int) *keyvals);
    return  std::wstring(1, (int) *keyvals);
  }

  //caps
  else if (( ss == Base ) && ( caps == 1 )) {
    GdkModifierType MOD_Caps = (GdkModifierType) ( GDK_LOCK_MASK );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_Caps, 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  //ALT-GR
  else if (( ss == MenuCtrl ) && ( caps == 0 )){
    GdkModifierType MOD_AltGr = (GdkModifierType) ( 144 );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_AltGr , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }


  //ALT-GR
  else if (( ss == MenuCtrl ) && ( caps == 1 )){
    GdkModifierType MOD_AltGr = (GdkModifierType) ( 144 );
    gdk_keymap_translate_keyboard_state (keymap, keycode, MOD_AltGr , 0, keyvals, NULL, NULL, & consumed);
    return  std::wstring(1, (int) *keyvals);
  }

  else
    return L"\0";
    //return L"0";
}

// _S2 maybe not needed
UINT find_SC_Other_from_SC_US_GDK(UINT SC_US,GdkKeymap *keymap) {
  UINT SC__Other;

    for ( int i=0; i<255;i++) {
      SC__Other= (UINT )get_KeyCode_From_KeyVal_GDK( keymap, i);
      if(SC__Other==SC_US )
        return SC__Other;
    }
  return SC_US;
}
