#include "keymap.h"

/*
static void PrintKeymapForCode(GdkKeymap *keymap, guint keycode)
{
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return;

  for (int i = 0; i < count; i++) {
    if (maps[i].level > 0 || maps[i].group > 1)
      continue;
    printf("    i=%d, keycode=%d, keyval=%d (%c), level=%d, group=%d\n", i, maps[i].keycode, keyvals[i], keyvals[i], maps[i].level, maps[i].group);
  }

  g_free(keyvals);
  g_free(maps);
}
*/


int dummytest_keymap(){
  std::cout<< " dummytest_keymap    is available\t";
  return 0;
}

bool write_US_ToVector( v_str_3D &vec,std::string language, const char* text) {
  std::string FullPathName = "/usr/share/X11/xkb/symbols/" + language;

  const char* path = FullPathName.c_str();
  FILE* fp = fopen((path), "r");
  if ( !fp) {
    printf("could not open file!");
    return 1;
  }

  // create 1D-vector of the complete line
  v_str_1D Vector_completeUS;
  if( CreateCompleteRow_US(Vector_completeUS,fp , text, language)) {
    printf("ERROR: can't Create complete row US \n");
    return 1;
  }

  // split contents of 1D Vector to 3D vector
  if( Split_US_To_3D_Vector( vec,Vector_completeUS)) {
    printf("ERROR: can't Split USto 3D-Vector \n");
    return 1;
  }

  printf("+++++++ dimensions of Vector after write_US_ToVector\t\t %li..%li..%li\n", vec.size(), vec[0].size(),vec[0][0].size());
  fclose(fp);
  return 0;
}

bool  CreateCompleteRow_US(v_str_1D &complete_List, FILE* fp, const char* text, std::string language) {
  // in the Configuration file we find the appopriate paragraph between "xkb_symbol <text>" and the next xkb_symbol
  // and then copy all rows starting with "key <" to a v1D-Vector

  int buffer_size = 512;
  char buffer[buffer_size];
  bool print_OK   = false;
  const char* key = "key <";
  std::string str_txt(text);
  std::string xbk_mark = "xkb_symbol";
  // TODO define folder to store File in
  std::ofstream KeyboardFile("File_" + language + ".txt");

  printf("Keyboard %s\n", text);
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
        printf("%s", buffer);
        complete_List.push_back(buffer);
        KeyboardFile << buffer;
      }
    }
  }

  if (complete_List.size() <1) {
    printf("ERROR: can't Create complete row US \n");
    return 1;
  }
  printf("-°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° \n");
  return 0;
}

bool Split_US_To_3D_Vector(v_str_3D &all_US,v_str_1D completeList) {
  // 1: take the whole line of the 1D-Vector and remove unwanted characters.
  // 2: seperate the name e.g. key<AD06> from the shiftstates
  // 3: push Names/Shiftstates to shift_states and then shiftstates to All_US, our 3D-Vector holding all Elements

  std::vector<char> delim{' ', '[', ']', '}', ';', '\t', '\n'};
  char split_bracel = '{';
  char split_char_komma  = ',';
  std::string empty  = "--";
  v_str_1D tokens;
  v_str_2D shift_states;

  // go through the whole vector
  for (int k = 0; k < (int)completeList.size() - 1; k++) {

    // remove all unwanted char
    for (int i = 0; i < (int) delim.size(); i++) {
      completeList[k].erase(remove(completeList[k].begin(), completeList[k].end(), delim[i]), completeList[k].end());
    }

    // only lines with ("key<.. are of interest
    if (completeList[k].find("key<") != std::string::npos) {

      //split off the key names
      std::istringstream split1(completeList[k]);
      for (std::string each; std::getline(split1, each, split_bracel); tokens.push_back(each));

      // replace keys names with number (<AD06> with 29,...)
      int Keycde = replace_PosKey_with_Keycode(tokens[0]);
      tokens[0] = std::to_string(Keycde);

      // we use characters a-z, A_Z only at the moment
      if (foundCharacterInList(tokens[0])) {

        // seperate rest of the vector to its elements and push to 'tokens'
        std::istringstream split(tokens[1]);
        tokens.pop_back();

        for (std::string each; std::getline(split, each, split_char_komma); tokens.push_back(each));
        //printf("### 5 Split_US_To_3D_Vector: tokens: size:%li...tokens[0]-[4]:-name:%s\tShiftstates:%s--%s--%s--%s---.\n", tokens.size(),tokens[0].c_str(),tokens[1].c_str(),tokens[2].c_str(),tokens[3].c_str(),tokens[4].c_str());

        // at the moment we only use the first 2 shiftstates (non-shift+shift) so get rid of all others
        int surplus = tokens.size() - shift_state_count -1;
        for( int j=0; j < surplus;j++) {
          tokens.pop_back();
        }

        // now push result to shift_states
        shift_states.push_back(tokens);
      }
      tokens.clear();
    }
  }
  all_US.push_back(shift_states);

  if ( all_US.size() ==0) {
    printf("ERROR: Can't split US to 3D-Vector\n");
    return 1;
  }
  //printf("### 6 Split_US_To_3D_clearVector %li..%li..%li\n", all_US.size(), all_US[0].size(),all_US[0][0].size());
  return 0;
}

bool foundCharacterInList(std::string tok) {
  //TOASK Do we need more and which?
  //US keys:          Q    W    E    R    T    Y    U    I    O    P    A    S    D    F    G    H    J    K    L    :    Z    X    C    V    B    N    M
  v_str_1D Keysyms { "24","25","26","27","28","29","30","31","32","33","38","39","40","41","42","43","44","45","46","47","52","53","54","55","56","57","58"};

  for ( int i =0; i< (int) Keysyms.size();i++) {
    //std::cout << "foundCharacterInList" << i <<"\n";
    if( tok == Keysyms[i]) {
      return true;
      }
    }
  return false;
}

int replace_PosKey_with_Keycode(std::string  in) {
  int out=0;
  if      ( in == "key<TLDE>")    out = 49;   // TOASK correct ???
  else if ( in == "key<AE01>")    out = 10;
  else if ( in == "key<AE02>")    out = 11;
  else if ( in == "key<AE03>")    out = 12;
  else if ( in == "key<AE04>")    out = 13;
  else if ( in == "key<AE05>")    out = 14;
  else if ( in == "key<AE06>")    out = 15;
  else if ( in == "key<AE07>")    out = 16;
  else if ( in == "key<AE08>")    out = 17;
  else if ( in == "key<AE09>")    out = 18;
  else if ( in == "key<AE10>")    out = 19;
  else if ( in == "key<AE11>")    out = 20;
  else if ( in == "key<AE12>")    out = 21;

  else if ( in == "key<AD01>")    out = 24;
  else if ( in == "key<AD02>")    out = 25;
  else if ( in == "key<AD03>")    out = 26;
  else if ( in == "key<AD04>")    out = 27;
  else if ( in == "key<AD05>")    out = 28;
  else if ( in == "key<AD06>")    out = 29;
  else if ( in == "key<AD07>")    out = 30;
  else if ( in == "key<AD08>")    out = 31;
  else if ( in == "key<AD09>")    out = 32;
  else if ( in == "key<AD10>")    out = 33;
  else if ( in == "key<AD11>")    out = 34;
  else if ( in == "key<AD12>")    out = 35;

  else if ( in == "key<AC01>")    out = 38;
  else if ( in == "key<AC02>")    out = 39;
  else if ( in == "key<AC03>")    out = 40;
  else if ( in == "key<AC04>")    out = 41;
  else if ( in == "key<AC05>")    out = 42;
  else if ( in == "key<AC06>")    out = 43;
  else if ( in == "key<AC07>")    out = 44;
  else if ( in == "key<AC08>")    out = 45;
  else if ( in == "key<AC09>")    out = 46;
  else if ( in == "key<AC10>")    out = 47;
  else if ( in == "key<AC11>")    out = 48;
  else if ( in == "key<AC12>")    out = 49;

  else if ( in == "key<AB01>")    out = 52;
  else if ( in == "key<AB02>")    out = 53;
  else if ( in == "key<AB03>")    out = 54;
  else if ( in == "key<AB04>")    out = 55;
  else if ( in == "key<AB05>")    out = 56;
  else if ( in == "key<AB06>")    out = 57;
  else if ( in == "key<AB07>")    out = 58;
  else if ( in == "key<AB08>")    out = 59;
  else if ( in == "key<AB09>")    out = 60;
  else if ( in == "key<AB10>")    out = 61;
  else if ( in == "key<BKSL>")    out = 62;   //TOASK correct ???
  else if ( in == "key<LSGT>")    out = 51;   //TOASK correct ???
  return out;
}

bool append_other_ToVector(v_str_3D &All_Vector,GdkKeymap * keymap) {

  // create a 2D vector all filled with "--" and push to 3D-Vector
  v_str_2D Other_Vector2D = create_empty_2D(All_Vector[0].size(),All_Vector[0][0].size());

  if (Other_Vector2D.size()==0) {
    printf("ERROR: create empty 2D-Vector failed");
    return 1;
  }

  All_Vector.push_back(Other_Vector2D);
  printf("+++++++ dimensions of Vector after append_other_ToVector\t %li..%li..%li\n", All_Vector.size(), All_Vector[0].size(),All_Vector[0][0].size());  

  if (All_Vector.size()==0) {
    printf("ERROR: creation of 3D-Vector failed");
    return 1;
  }

  for(int i =0; i< (int) All_Vector[1].size();i++) {

    // get key name US stored in [0][i][0] and copy to name in other-block[1][i][0]
    All_Vector[1][i][0] = All_Vector[0][i][0];

    // write this value to 3D- Vector
    All_Vector[1][i][0+1] = GetKeyvalsFromKeymap(keymap,stoi(All_Vector[1][i][0]),0);   //shift state: unshifted:0
    All_Vector[1][i][1+1] = GetKeyvalsFromKeymap(keymap,stoi(All_Vector[1][i][0]),1);   //shift state: shifted:1

    printf(" Keycodes US->Other:   %d(US): %s %s ---- (other):%s,    \n",stoi(All_Vector[1][i][0]),All_Vector[0][i][1].c_str(),All_Vector[0][i][2].c_str(),All_Vector[1][i][1].c_str());  
  }

  return 0;
}

v_str_2D create_empty_2D( int dim_rows,int dim_shifts) {
  std::string empty = "--";
  v_str_1D shifts;
  v_str_2D all;

  for ( int i=0; i< dim_rows;i++) {
    for ( int j=0; j< dim_shifts;j++) {
      shifts.push_back(empty);
    }
    all.push_back(shifts);
    shifts.clear();
  }
  return all;
}

int GetKeyvalsFromKeymap(GdkKeymap *keymap, guint keycode, int shift_state_pos) {
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;
  int out;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return 0;
  //if(!gdk_wayland_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
  //  return 0;

  if (!(shift_state_pos < count))
    return 0;

  out = keyvals[shift_state_pos];

  g_free(keyvals);
  g_free(maps);
  return out;
}

bool extract_difference( v_str_3D &All_Vector) {
  // TODO define which Folder; find better name
  std::ofstream Map_File("Map_US.txt");
  std::string diff =" ";

  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  std::cout << "Nr of \n" ;
  std::cout << "Key: " <<  "\t Character US (no shift) " <<  "  Character US (shift) "<<  "\t\tCharacter other (no shift)" <<  "\tCharacter other (shift)   difference \n" ;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");

  Map_File <<"--------------------------------------------------------------------------------------------------------------------------------------------\n";
  Map_File << "Nr of \n" ;
  Map_File << "Key: " <<  "\t Character US (no shift) " <<  "  Character US (shift) "<<  "\t\tCharacter other (no shift)" <<  "\tCharacter other (shift)   difference \n" ;
  Map_File <<"--------------------------------------------------------------------------------------------------------------------------------------------\n";

  for ( int k=0; k<(int)All_Vector[0].size(); k++) {
    if (All_Vector[0][k][1] == All_Vector[1][k][1])
      diff =" ";
    else
      diff =" *** ";

    std::cout << All_Vector[0][k][0] << "\t " <<+(*(All_Vector[0][k][1].c_str()))<< "\t("<< All_Vector[0][k][1] <<")"<<std::setw(20-All_Vector[0][k][1].size())<<  +(*(All_Vector[0][k][2].c_str()))<< "\t("<< All_Vector[0][k][2] <<")"<<std::setw(20-All_Vector[0][k][2].size())<< "\t...\t"<< +(*(All_Vector[1][k][1].c_str()))<< "\t("<< All_Vector[1][k][1] <<")"<<std::setw(24-All_Vector[1][k][1].size())<<   +(*(All_Vector[1][k][2].c_str()))<< "\t("<< All_Vector[1][k][2] <<")"<<std::setw(20-All_Vector[1][k][2].size())<< diff <<"\n";
    Map_File  << All_Vector[0][k][0] << " \t "<<+(*(All_Vector[0][k][1].c_str()))<< "\t("<< All_Vector[0][k][1] <<")"<<std::setw(20-All_Vector[0][k][1].size())<<  +(*(All_Vector[0][k][2].c_str()))<< "\t("<< All_Vector[0][k][2] <<")"<<std::setw(20-All_Vector[0][k][2].size())<< "\t...\t"<< +(*(All_Vector[1][k][1].c_str()))<< "\t("<< All_Vector[1][k][1] <<")"<<std::setw(24-All_Vector[1][k][1].size())<<   +(*(All_Vector[1][k][2].c_str()))<< "\t("<< All_Vector[1][k][2] <<")"<<std::setw(20-All_Vector[1][k][2].size())<< diff << "\n";
  }

  std::streampos fsize = Map_File.tellp();
  if ( fsize <1) {
    printf("ERROR: can't extract difference\n");
    Map_File.close();
    return 1;
  }

  Map_File.close();
  return 0;
}

std::string get_Other_Char_FromUS( std::string in , v_str_3D &All_Vector) {
  std::string diff;
  // find correct row of char in US
  for( int i=0; i< (int) All_Vector[0].size();i++) {
    for( int j=1; j< (int)All_Vector[0][0].size();j++) {

      //std::cout << "see and compare:  "<<  "All_Vector[0]["<<i<<"]["<<j<<"] :"<< All_Vector[0][i][j].c_str() << " in: " << in<<"\n";
      if  ( All_Vector[0][i][j] == in ) {
        if ( All_Vector[0][i][j] != All_Vector[1][i][j])
          diff =" **  ";
        std::cout << "US -> Other: "<<  std::setw(5)<<diff<<All_Vector[0][i][j] << std::setw(15-All_Vector[0][i][j].size())<< All_Vector[1][i][j] <<"\n";
        return All_Vector[1][i][j] ;
      }
    }
  }
  std::cout << "US -> Other:      (" <<  in << ": no match)\n";
  return "-";
}

std::string get_US_Char_FromOther(std::string in , v_str_3D &All_Vector) {
  std::string diff;
  // find correct row of char in other
  for( int i=0; i< (int)All_Vector[1].size();i++) {
    for( int j=1; j< (int)All_Vector[1][0].size();j++) {
      if  ( All_Vector[1][i][j] == in ) {
        if ( All_Vector[0][i][j] != All_Vector[1][i][j])
          diff =" **  ";
        std::cout << "Other -> US: "<<  std::setw(5)<<diff<<All_Vector[1][i][j] << std::setw(15-All_Vector[0][i][j].size())<<  All_Vector[0][i][j] <<"\n";
        return All_Vector[0][i][j];
        }
      }
    }
    std::cout << "Other -> US:      (" <<  in << ": no match)\n";
    return "-";
  }

std::string getKeyNrOf_USChar(std::string in , v_str_3D &All_Vector) {
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[0].size();i++) {
    for( int j=1; j< (int)All_Vector[0][0].size();j++) {
      if  ( All_Vector[0][i][j] == in ) {
        std::cout << "KeyNr of US char: \t"<< All_Vector[0][i][j] << " -> " << All_Vector[0][i][0] <<"\n";
        return All_Vector[0][i][0] ;
      }
    }
  }
  return "-";
}

std::string getKeyNrOf_OtherChar(std::string in , v_str_3D &All_Vector) {
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[1].size();i++) {
    for( int j=1; j< (int)All_Vector[1][0].size();j++) {
      if  ( All_Vector[1][i][j] == in ) {
        std::cout << "KeyNr of Other char : \t"<< All_Vector[1][i][j] << " -> " << All_Vector[1][i][0] <<"\n";
        return All_Vector[1][i][0] ;
      }
    }
  }
  return "-";
}

bool test(v_str_3D &V) {
  printf("+++++++ dimensions of Vector in test()\t\t %li..%li..%li\n", V.size(), V[0].size(),V[0][0].size());
  printf("\n+++++++++ print some characters of US and Other +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  for ( int k=0; k<(int)V[0].size(); k++) {
    std::cout   << " row 1 (US)......" << V[0][k][0]<< ".."  << V[0][k][1]<< ".."<<  "..\n"   ;
    if (V.size()>1)
      std::cout << "   row 1 (Other).."<< V[1][k][0]<< ".."  << V[1][k][1]<< ".."<<  "..\n" ;
  }
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");        
  return true;
}

void test_in_out(v_str_3D &All_Vector) {
std::string diff;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
   //checks mapping between US and other
  std::string a = get_Other_Char_FromUS( "z", All_Vector);
  std::string aa = get_Other_Char_FromUS( "Z", All_Vector);
  std::string aaa = get_Other_Char_FromUS( "y", All_Vector);
  std::string aaaa = get_Other_Char_FromUS( "Y", All_Vector);

  std::string b = get_US_Char_FromOther( "z", All_Vector);
  std::string bb = get_US_Char_FromOther( "Z", All_Vector);
  std::string bbb = get_US_Char_FromOther( "y", All_Vector);
  std::string bbbb = get_US_Char_FromOther( "Y", All_Vector);

  std::string c = getKeyNrOf_OtherChar( "z", All_Vector);
  std::string cc = getKeyNrOf_OtherChar( "Z", All_Vector);
  std::string ccc = getKeyNrOf_OtherChar( "y", All_Vector);
  std::string cccc = getKeyNrOf_OtherChar( "Y", All_Vector);

  std::string d = getKeyNrOf_USChar( "z", All_Vector);
  std::string dd = getKeyNrOf_USChar( "Z", All_Vector);
  std::string ddd = getKeyNrOf_USChar( "y", All_Vector);
  std::string dddd = getKeyNrOf_USChar( "Y", All_Vector);

  std::cout << "get_Other_Char_FromUS z-Z-y-Y: "  << ".." << a<< ".." <<aa<< ".." <<aaa<< ".." <<aaaa<< ".." << "\n";
  std::cout << "get_US_Char_FromOther z-Z-y-Y: "  << ".." << b<< ".." <<bb<< ".." <<bbb<< ".." <<bbbb<< ".." << "\n";
  std::cout << "getKeyNrOf_OtherChar z-Z-y-Y: "   << ".." << c<< ".." <<cc<< ".." <<ccc<< ".." <<cccc<< ".." << "\n";
  std::cout << "getKeyNrOf_USChar z-Z-y-Y: "      << ".." << d<< ".." <<dd<< ".." <<ddd<< ".." <<dddd<< ".." << "\n";
}

void print_simple_map_US(v_str_3D &All_Vector, int shiftstate) {
  std::string out;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  for ( int i=0; i< (int)All_Vector[0].size();i++) {
    out =get_Other_Char_FromUS(All_Vector[0][i][shiftstate], All_Vector);
  }
}

void print_simple_map_Other(v_str_3D &All_Vector, int shiftstate) {
  std::string out;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  for ( int i=0; i< (int)All_Vector[0].size();i++) {
    out = get_US_Char_FromOther(All_Vector[0][i][shiftstate], All_Vector);
  }
}

void test_specific_Characters(v_str_3D &All_Vector) {
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  v_str_1D in {"a", "b", "m", "w", "x", "y", "z"};
  std::string  out;
  for( int i=0; i< (int) in.size()-1; i++) {
    out = get_Other_Char_FromUS(in[i], All_Vector);
  }
}

//TODO needs to take 2 keyboards as input
int run_DoConvert_Part1_getMap(gint argc, gchar *argv[]) {

  gdk_init(&argc, &argv);
  GdkDisplay *display = gdk_display_get_default();
  if (!display) {
    printf("ERROR: can't get display\n");
    return 1;
  }
  GdkKeymap *keymap = gdk_keymap_get_for_display(display);
  if (!keymap) {
    printf("ERROR: Can't get keymap\n");
    gdk_display_close(display);
    return 2;
  }

  // write content of xkb_symbols to 3D Vector
  // I assume we use Keyboard US basic as base
  std::string US_language    = "us";
  const char* text_us        = "xkb_symbols \"basic\"";

  v_str_3D All_Vector;
  if(write_US_ToVector(All_Vector,US_language, text_us)) {
    printf("ERROR: can't write US to Vector \n");
    return 1;
  }
  test(All_Vector);

  // add contents of other keyboard to vector
  if( append_other_ToVector(All_Vector,keymap)) {
    printf("ERROR: can't append other ToVector \n");
    return 1;
  }
  test(All_Vector);

  if( extract_difference(All_Vector)) {
    printf("ERROR: can't extract difference \n");
    return 1;
  }

  test_in_out(All_Vector);
  print_simple_map_US(All_Vector,1);    // 1 = non-shift, 2 = shift
  print_simple_map_Other(All_Vector,2); // 1 = non-shift, 2 = shift
  test_specific_Characters(All_Vector);

  gdk_display_close(display);

  printf("°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° end\n");
  return 0;
}

void LoadKeyboard() {std::cout << "#### LoadKeyboard not implemented yet ! \n";}

void run_DoConvert_Part2_TranslateKeyboard() {std::cout << "#### TranslateKeyboard not implemented yet ! \n";}

void SaveKeyboard() {std::cout << "#### SaveKeyboard not implemented yet ! \n";}
//--------------------------------------

//int main(gint argc, gchar *argv[])    // this is for use of single keymap
/*int main_keymap(gint argc, gchar *argv[])   // this is for use of keymap together with mcompile
{
  check_avaiability_of_modules_();
  LoadKeyboard();
  std::cout << "############################################################################################### end LoadKeyboard \n";

  int out = run_DoConvert_Part1_getMap(argc,argv);
  run_DoConvert_Part2_TranslateKeyboard();
  SaveKeyboard();

  std::cout << "################################################################################################################################### end keymap \n";
  return out ;
}*/
// old ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
static void PrintKeymapForCode(GdkKeymap *keymap, guint keycode)
{
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return;

  for (int i = 0; i < count; i++) {
    if (maps[i].level > 0 || maps[i].group > 1)
      continue;
    printf("    i=%d, keycode=%d, keyval=%d (%c), level=%d, group=%d\n", i, maps[i].keycode, keyvals[i], keyvals[i], maps[i].level, maps[i].group);
  }

  g_free(keyvals);
  g_free(maps);
}
*/
/*
void write_US_ToVector( v_str_3D &vec,std::string language, const char* text) {
  // ? CHECK if ran OK-> return 0/1
  std::string FullPathName = "/usr/share/X11/xkb/symbols/" + language;

  const char* path = FullPathName.c_str();
  FILE* fp = fopen((path), "r");
  if ( !fp)
    printf("could not open file!");

  // create 1D-vector of the complete line
  v_str_1D Vector_completeUS;
  CreateCompleteRow_US(Vector_completeUS,fp , text, language);

  // split contents of 1D Vector to 3D vector
  Split_US_To_3D_Vector( vec,Vector_completeUS);

  printf("+++++++ dimensions of Vector after write_US_ToVector\t\t %li..%li..%li\n", vec.size(), vec[0].size(),vec[0][0].size());
  fclose(fp);
}

void  CreateCompleteRow_US(v_str_1D &complete_List, FILE* fp, const char* text, std::string language) {
  // in the Configuration file we find the appopriate paragraph between "xkb_symbol <text>" and the next xkb_symbol
  // and then copy all rows starting with "key <" to a v1D-Vector

  // ? CHECK if ran OK-> return 0/1
  int buffer_size = 512;
  char buffer[buffer_size];
  bool print_OK   = false;
  const char* key = "key <";
  std::string str_txt(text);
  std::string xbk_mark = "xkb_symbol";
  // TODO define folder to store File in
  std::ofstream KeyboardFile("File_" + language + ".txt");

  printf("Keyboard %s\n", text);
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
        printf("%s", buffer);
        complete_List.push_back(buffer);
        KeyboardFile << buffer;
      }
    }
  }
  printf("-°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° \n");
}

void Split_US_To_3D_Vector(v_str_3D &all_US,v_str_1D completeList) {
  // 1: take the whole line of the 1D-Vector and remove unwanted characters.
  // 2: seperate the name e.g. key<AD06> and the shiftstates
  // 3: push Names/Shiftstates to shift_states and then shiftstates to All_US, our 3D-Vector holding all Elements

  // ? CHECK if ran OK-> return 0/1
  std::vector<char> delim{' ', '[', ']', '}', ';', '\t', '\n'};
  char split_bracel = '{';
  char split_char_komma  = ',';
  std::string empty  = "--";
  v_str_1D tokens;
  v_str_2D shift_states;

  // go through the whole vector
  for (int k = 0; k < (int)completeList.size() - 1; k++) {

    // remove all unwanted char
    for (int i = 0; i < (int) delim.size(); i++) {
      completeList[k].erase(remove(completeList[k].begin(), completeList[k].end(), delim[i]), completeList[k].end());
    }

    // only lines with ("key<.. are of interest
    if (completeList[k].find("key<") != std::string::npos) {

      //split off the key names
      std::istringstream split1(completeList[k]);
      for (std::string each; std::getline(split1, each, split_bracel); tokens.push_back(each));

      // replace keys names with number (<AD06> with 29,...)

      // ? CHECK if ran OK-> return 0/1
      int Keycde = replace_PosKey_with_Keycode(tokens[0]);
      tokens[0] = std::to_string(Keycde);

      // seperate rest of the vector to its elements and push to 'tokens'
      std::istringstream split(tokens[1]);
      tokens.pop_back();
      for (std::string each; std::getline(split, each, split_char_komma); tokens.push_back(each));
      //printf("### 5 Split_US_To_3D_Vector: tokens: size:%li...tokens[0]-[4]:-name:%s\tShiftstates:%s--%s--%s--%s---.\n", tokens.size(),tokens[0].c_str(),tokens[1].c_str(),tokens[2].c_str(),tokens[3].c_str(),tokens[4].c_str());

      // at the moment we only use the first 2 shiftstates (non-shift+shift) so get rid of all others
      int surplus = tokens.size() - shift_state_count -1;
      for( int j=0; j < surplus;j++) {
        tokens.pop_back();
      }

      // now push result to shift_states
      shift_states.push_back(tokens);
      tokens.clear();
    }
  }
  all_US.push_back(shift_states);

  // ? CHECK if ran OK, vector size is correct -> return 0/1
  //printf("### 6 Split_US_To_3D_clearVector %li..%li..%li\n", all_US.size(), all_US[0].size(),all_US[0][0].size());
}

int replace_PosKey_with_Keycode(std::string  in) {
  int out=0;
  if      ( in == "key<TLDE>")    out = 49;   //correct ???
  else if ( in == "key<AE01>")    out = 10;
  else if ( in == "key<AE02>")    out = 11;
  else if ( in == "key<AE03>")    out = 12;
  else if ( in == "key<AE04>")    out = 13;
  else if ( in == "key<AE05>")    out = 14;
  else if ( in == "key<AE06>")    out = 15;
  else if ( in == "key<AE07>")    out = 16;
  else if ( in == "key<AE08>")    out = 17;
  else if ( in == "key<AE09>")    out = 18;
  else if ( in == "key<AE10>")    out = 19;
  else if ( in == "key<AE11>")    out = 20;
  else if ( in == "key<AE12>")    out = 21;

  else if ( in == "key<AD01>")    out = 24;
  else if ( in == "key<AD02>")    out = 25;
  else if ( in == "key<AD03>")    out = 26;
  else if ( in == "key<AD04>")    out = 27;
  else if ( in == "key<AD05>")    out = 28;
  else if ( in == "key<AD06>")    out = 29;
  else if ( in == "key<AD07>")    out = 30;
  else if ( in == "key<AD08>")    out = 31;
  else if ( in == "key<AD09>")    out = 32;
  else if ( in == "key<AD10>")    out = 33;
  else if ( in == "key<AD11>")    out = 34;
  else if ( in == "key<AD12>")    out = 35;

  else if ( in == "key<AC01>")    out = 38;
  else if ( in == "key<AC02>")    out = 39;
  else if ( in == "key<AC03>")    out = 40;
  else if ( in == "key<AC04>")    out = 41;
  else if ( in == "key<AC05>")    out = 42;
  else if ( in == "key<AC06>")    out = 43;
  else if ( in == "key<AC07>")    out = 44;
  else if ( in == "key<AC08>")    out = 45;
  else if ( in == "key<AC09>")    out = 46;
  else if ( in == "key<AC10>")    out = 47;
  else if ( in == "key<AC11>")    out = 48;
  else if ( in == "key<AC12>")    out = 49;

  else if ( in == "key<AB01>")    out = 52;
  else if ( in == "key<AB02>")    out = 53;
  else if ( in == "key<AB03>")    out = 54;
  else if ( in == "key<AB04>")    out = 55;
  else if ( in == "key<AB05>")    out = 56;
  else if ( in == "key<AB06>")    out = 57;
  else if ( in == "key<AB07>")    out = 58;
  else if ( in == "key<AB08>")    out = 59;
  else if ( in == "key<AB09>")    out = 60;
  else if ( in == "key<AB10>")    out = 61;
  else if ( in == "key<BKSL>")    out = 62;   //correct ???
  else if ( in == "key<LSGT>")    out = 51;   //correct ???
  return out;
}

void append_other_ToVector(v_str_3D &All_Vector,GdkKeymap * keymap) {

  // create a 2D vector all fill0ed with "--" and push to 3D-Vector
  // ? CHECK if ran OK-> return 0/1
  v_str_2D Other_Vector2D = create_empty_2D(All_Vector[0].size(),All_Vector[0][0].size());
  All_Vector.push_back(Other_Vector2D);

  printf("+++++++ dimensions of Vector after append_other_ToVector\t %li..%li..%li\n", All_Vector.size(), All_Vector[0].size(),All_Vector[0][0].size());  

  for(int i =1; i< (int) All_Vector[1].size()-1;i++)
  {
    // get key name US stored in [0][i][0] and copy to name in other-block[1][i][0]
    All_Vector[1][i][0] = All_Vector[0][i][0];

    // write this value to 3D- Vector
    All_Vector[1][i][0+1] = GetKeyvalsFromKeymap(keymap,stoi(All_Vector[1][i][0]),0);   //shift state: unshifted:0
    All_Vector[1][i][1+1] = GetKeyvalsFromKeymap(keymap,stoi(All_Vector[1][i][0]),1);   //shift state: shifted:1
    //printf("Keycodes US->Other:   %d(US): %s %s ---- (other):%s,  %s,  %s  \n",stoi(All_Vector[1][i][0]),All_Vector[0][i][1].c_str(),All_Vector[0][i][2].c_str(),All_Vector[1][i][1].c_str(),All_Vector[1][i][2].c_str(),All_Vector[1][i][3].c_str());  
  }
  // ? CHECK if ran OK, vector size is correct -> return 0/1
}

v_str_2D create_empty_2D( int dim_rows,int dim_shifts)
{
  std::string empty = "--";
  v_str_1D shifts;
  v_str_2D all;

  for ( int i=0; i< dim_rows;i++) {
    for ( int j=0; j< dim_shifts;j++) {
      shifts.push_back(empty);
    }
    all.push_back(shifts);
    shifts.clear();
  }
  //printf("+++++++ dimensions of Vector after create_empty_2D\t\t %li..%li..%li\n", all.size(), all[0].size(),all[1].size());
  return all;
}

int GetKeyvalsFromKeymap(GdkKeymap *keymap, guint keycode, int shift_state_pos) {
  GdkKeymapKey *maps;
  guint *keyvals;
  gint count;
  int out;

  if (!gdk_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
    return 0;
  //if(!gdk_wayland_keymap_get_entries_for_keycode(keymap, keycode, &maps, &keyvals, &count))
  //  return 0;

  if (!(shift_state_pos < count))
    return 0;

  out = keyvals[shift_state_pos];

  g_free(keyvals);
  g_free(maps);
  return out;
}

void extract_difference( v_str_3D &All_Vector)
{
  // ? CHECK if ran OK-> return 0/1
  // TODO define which Folder; find better name
  std::ofstream Map_File("Map_US.txt");
  std::string diff =" ";

  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  std::cout << "Nr of \n" ;
  std::cout << "Key: " <<  "\t Character US (no shift) " <<  "  Character US (shift) "<<  "\t\tCharacter other (no shift)" <<  "\tCharacter other (shift)   difference \n" ;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");

  Map_File <<"--------------------------------------------------------------------------------------------------------------------------------------------\n";
  Map_File << "Nr of \n" ;
  Map_File << "Key: " <<  "\t Character US (no shift) " <<  "  Character US (shift) "<<  "\t\tCharacter other (no shift)" <<  "\tCharacter other (shift)   difference \n" ;
  Map_File <<"--------------------------------------------------------------------------------------------------------------------------------------------\n";

  for ( int k=0; k<(int)All_Vector[0].size()-1; k++) {
    if (All_Vector[0][k][1] == All_Vector[1][k][1])
      diff =" ";
    else
      diff =" *** ";
    // ? CHECK if index exists
    std::cout << All_Vector[0][k][0] << "\t " <<+(*(All_Vector[0][k][1].c_str()))<< "\t("<< All_Vector[0][k][1] <<")"<<std::setw(20-All_Vector[0][k][1].size())<<  +(*(All_Vector[0][k][2].c_str()))<< "\t("<< All_Vector[0][k][2] <<")"<<std::setw(20-All_Vector[0][k][2].size())<< "\t...\t"<< +(*(All_Vector[1][k][1].c_str()))<< "\t("<< All_Vector[1][k][1] <<")"<<std::setw(24-All_Vector[1][k][1].size())<<   +(*(All_Vector[1][k][2].c_str()))<< "\t("<< All_Vector[1][k][2] <<")"<<std::setw(20-All_Vector[1][k][2].size())<< diff <<"\n";
    Map_File  << All_Vector[0][k][0] << " \t "<<+(*(All_Vector[0][k][1].c_str()))<< "\t("<< All_Vector[0][k][1] <<")"<<std::setw(20-All_Vector[0][k][1].size())<<  +(*(All_Vector[0][k][2].c_str()))<< "\t("<< All_Vector[0][k][2] <<")"<<std::setw(20-All_Vector[0][k][2].size())<< "\t...\t"<< +(*(All_Vector[1][k][1].c_str()))<< "\t("<< All_Vector[1][k][1] <<")"<<std::setw(24-All_Vector[1][k][1].size())<<   +(*(All_Vector[1][k][2].c_str()))<< "\t("<< All_Vector[1][k][2] <<")"<<std::setw(20-All_Vector[1][k][2].size())<< diff << "\n";
  }

  // ? CHECK if ran OK, return 0/1
  Map_File.close();
}

std::string get_Other_Char_FromUS( std::string in , v_str_3D &All_Vector) {
  std::string diff;
  // find correct row of char in US
  for( int i=0; i< (int) All_Vector[0].size()-1;i++) {
    for( int j=0; j< (int)All_Vector[0][0].size()-1;j++) {
      if  ( All_Vector[0][i][j] == in ) {
        if ( All_Vector[0][i][j] != All_Vector[1][i][j])
          diff =" **  ";
          // ? CHECK if Index exists
        std::cout << "US -> Other: "<<  std::setw(5)<<diff<<All_Vector[0][i][j] << std::setw(15-All_Vector[0][i][j].size())<< All_Vector[1][i][j] <<"\n";
        return All_Vector[1][i][j] ;
      }
    }
  }
  std::cout << "US -> Other:      (" <<  in << ": no match)\n";
  return "-";
}

std::string get_US_Char_FromOther(std::string in , v_str_3D &All_Vector) {
  std::string diff;
  // find correct row of char in other
  for( int i=0; i< (int)All_Vector[1].size()-1;i++) {
    for( int j=0; j< (int)All_Vector[1][0].size()-1;j++) {
      if  ( All_Vector[1][i][j] == in ) {
        if ( All_Vector[0][i][j] != All_Vector[1][i][j])
          diff =" **  ";
        // ? CHECK if Index exists
        std::cout << "Other -> US: "<<  std::setw(5)<<diff<<All_Vector[1][i][j] << std::setw(15-All_Vector[0][i][j].size())<<  All_Vector[0][i][j] <<"\n";
        return All_Vector[0][i][j];
        }
      }
    }
    std::cout << "Other -> US:      (" <<  in << ": no match)\n";
    return "-";
  }

std::string getKeyNrOf_USChar(std::string in , v_str_3D &All_Vector) {
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[0].size()-1;i++) {
    for( int j=0; j< (int)All_Vector[0][0].size()-1;j++) {
      if  ( All_Vector[0][i][j] == in ) {
        // ? CHECK if index exists
        std::cout << "KeyNr of US char: \t"<< All_Vector[0][i][j] << " -> " << All_Vector[0][i][0] <<"\n";
        return All_Vector[0][i][0] ;
      }
    }
  }
  return "-";
}

std::string getKeyNrOf_OtherChar(std::string in , v_str_3D &All_Vector) {
  // find correct row of char in US
  for( int i=0; i< (int)All_Vector[1].size()-1;i++) {
    for( int j=0; j< (int)All_Vector[1][0].size()-1;j++) {
      if  ( All_Vector[1][i][j] == in ) {
        // ? CHECK if index exists
        std::cout << "KeyNr of Other char : \t"<< All_Vector[1][i][j] << " -> " << All_Vector[1][i][0] <<"\n";
        return All_Vector[1][i][0] ;
      }
    }
  }
  return "-";
}

bool test(v_str_3D &V) {
// ? CHECK if index exists
  printf("\n+++++++++ print some characters of US and Other +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  for ( int k=13; k<43; k++) {
    std::cout   << " row 1 (US)......" << V[0][k][0]<< ".."  << V[0][k][1]<< ".."<< V[0][k][2]<< ".."  << V[0][k][3]<< ".."  << V[0][k][4]<< "..\n"   ;
    if (V.size()>1)
      std::cout << "   row 1 (Other).."<< V[1][k][0]<< ".."  << V[1][k][1]<< ".."<< V[1][k][2]<< ".."  << V[1][k][3]<< ".."  << V[1][k][4]<< "..\n" ;
  }
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");        
  return true;
}

void test_in_out(v_str_3D &All_Vector) {
std::string diff;
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
   //checks mapping between US and other
  std::string a = get_Other_Char_FromUS( "z", All_Vector);
  std::string aa = get_Other_Char_FromUS( "Z", All_Vector);
  std::string aaa = get_Other_Char_FromUS( "y", All_Vector);
  std::string aaaa = get_Other_Char_FromUS( "Y", All_Vector);

  std::string b = get_US_Char_FromOther( "z", All_Vector);
  std::string bb = get_US_Char_FromOther( "Z", All_Vector);
  std::string bbb = get_US_Char_FromOther( "y", All_Vector);
  std::string bbbb = get_US_Char_FromOther( "Y", All_Vector);

  std::string c = getKeyNrOf_OtherChar( "z", All_Vector);
  std::string cc = getKeyNrOf_OtherChar( "Z", All_Vector);
  std::string ccc = getKeyNrOf_OtherChar( "y", All_Vector);
  std::string cccc = getKeyNrOf_OtherChar( "Y", All_Vector);

  std::string d = getKeyNrOf_USChar( "z", All_Vector);
  std::string dd = getKeyNrOf_USChar( "Z", All_Vector);
  std::string ddd = getKeyNrOf_USChar( "y", All_Vector);
  std::string dddd = getKeyNrOf_USChar( "Y", All_Vector);

  std::cout << "get_Other_Char_FromUS z-Z-y-Y: "  << ".." << a<< ".." <<aa<< ".." <<aaa<< ".." <<aaaa<< ".." << "\n";
  std::cout << "get_US_Char_FromOther z-Z-y-Y: "  << ".." << b<< ".." <<bb<< ".." <<bbb<< ".." <<bbbb<< ".." << "\n";
  std::cout << "getKeyNrOf_OtherChar z-Z-y-Y: "   << ".." << c<< ".." <<cc<< ".." <<ccc<< ".." <<cccc<< ".." << "\n";
  std::cout << "getKeyNrOf_USChar z-Z-y-Y: "      << ".." << d<< ".." <<dd<< ".." <<ddd<< ".." <<dddd<< ".." << "\n";
}

void print_simple_map_US(v_str_3D &All_Vector, int shiftstate){
  std::string out,diff;
  // ? CHECK if ran OK-> return 0/1
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  for ( int i=0; i< (int)All_Vector[0].size();i++) {
    out =get_Other_Char_FromUS(All_Vector[0][i][shiftstate], All_Vector);
  }
}

void print_simple_map_Other(v_str_3D &All_Vector, int shiftstate){
  std::string out, diff;
  // ? CHECK if ran OK-> return 0/1
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  for ( int i=0; i< (int)All_Vector[0].size();i++) {
    out = get_US_Char_FromOther(All_Vector[0][i][shiftstate], All_Vector);
  }
}

void test_specific_Characters(v_str_3D &All_Vector){
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  v_str_1D in {"a", "b", "m", "w", "x", "y", "z"};
  std::string  out;
  for( int i=0; i< (int) in.size()-1; i++) {
    out = get_Other_Char_FromUS(in[i], All_Vector);
  }
}


//--------------------------------------
int main(gint argc, gchar *argv[])
{
  gdk_init(&argc, &argv);
  GdkDisplay *display = gdk_display_get_default();
  if (!display) {
    printf("ERROR: can't get display\n");
    return 1;
  }
  GdkKeymap *keymap = gdk_keymap_get_for_display(display);
  if (!keymap) {
    printf("ERROR: Can't get keymap\n");
    gdk_display_close(display);
    return 2;
  }

  // write content of xkb_symbols to 3D Vector
  // I assume we use Keyboard US basic as base
  std::string US_language    = "us";
  const char* text_us        = "xkb_symbols \"basic\"";

  v_str_3D All_Vector;
  write_US_ToVector(All_Vector,US_language, text_us);
  //test(All_Vector);

  // add contents of other keyboard to vector
  append_other_ToVector(All_Vector,keymap);
  //test(All_Vector);

  extract_difference(All_Vector);
  //test_in_out(All_Vector);

  //print_simple_map_US(All_Vector,1);    // 1 = non-shift
  //print_simple_map_Other(All_Vector,1); // 1 = non-shift
  test_specific_Characters(All_Vector);
  gdk_display_close(display);

  printf("°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° end\n");
  return 0;
}
*/