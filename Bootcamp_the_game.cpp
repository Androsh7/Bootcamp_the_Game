#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <windows.h>
using namespace std;

// Terminal Color Codes
// I.E: cout << Black << "My Text" << RST_color;
#define RST_color "\033[0m"
#define Black "\033[30m" // more like a grey on powershell
#define Blue "\033[34m"
#define Green "\033[32m"
#define Cyan "\033[36m"
#define Red "\033[31m"
#define Purple "\033[35m"
#define Brown "\033[33m"
#define Light_Gray "\033[37m"
#define Dark_Gray "\033[1;30m"
#define Light_Blue "\033[1;34m"
#define Light_Green "\033[1;32m"
#define Light_Cyan "\033[1;36m"
#define Light_Red "\033[1;31m"
#define Light_Purple "\033[1;35m"
#define Yellow "\033[1;33m"
#define White "\033[1;37m"

// converts a number to a status bar, so 55 would look like: "[===========         ]"
string generate_status_bar(int in_num, int max_num = 100, int increment_num = 5, char percent_char = '=', char start_char = '[', char end_char = ']', char pad_char = ' '){
    string outstring = "";
    int percent_num = in_num / increment_num; // number of percent chars to print
    int remainder = (max_num / increment_num) - percent_num; // number of pad chars to print
    
    // creates outstring
    outstring += start_char;
    for (int i = 0; i < percent_num; i++){
        outstring += percent_char;
    }
    for (int i = 0; i < remainder; i++){
        outstring += pad_char;
    }
    outstring += end_char;

    return outstring;
}

// draws a line
string generate_line(int length, char start_char = '<', char middle_char = '=', char end_char = '>') {
    string out_string = "";
    out_string += start_char;
    for (int i = 0; i < length - 2; i++){
        out_string += middle_char;
    }
    out_string += end_char;
    return out_string;
}

struct point{
    unsigned int x,y;
};

// class for a movable text box
class text_box {
public:
    string text = "";
    int type = 0; // 0 = left-aligned, 1 = right-aligned, 2 = center-aligned, 3 = downward
    point start_point; // start point for the text box, for right aligned text it will be printed from the rightmost edge of the text box
    int length;
    int height;

    update_text(string in_string, unsigned int str_length = 0) {
    int max_len = 0;
    if (str_length){
        int max_len = length;
    }
    else {
        int max_len = str_length;
    }
    int line_count = 1;
    string cut_string = "";
    for (int i = 0; i < in_string.size(); i++){
        if (in_string.at(i) == '\n'){
            max_len = max(max_len, int(cut_string.size()));
            line_count++;
            cut_string = "";
        }
        cut_string += in_string.at(i);
    }
    text = in_string;
    length = max_len;
    height = line_count;
    return 0;
    }

    // constructor
    text_box (unsigned int align_type, point set_point, unsigned int set_length = 0) {
        type = align_type;
        start_point = set_point;
        length = set_length;
    }
};

// includes tools to write using the cursor
class cursor_write {
public:
    // uses a terminal command to clear the screen
    int clear_screen() {
        printf("\033[2J");
        return 0;
    }

    // set the cursor position with the provided x,y cords
    int set_cursor(point set_cord) {
        printf("\033[%d;%dH", set_cord.y + 1, set_cord.x + 1);
        return 0;
    }

    int move_cursor_left(unsigned int x_change = 1) {
        printf("\033[%dD", x_change);
        return 0;
    }

    int move_cursor_right(unsigned int x_change = 1) {
        printf("\033[%dC", x_change);
        return 0;
    }

    int move_cursor_up(unsigned int y_change = 1) {
        printf("\033[%dA", y_change);
        return 0;
    }

    int move_cursor_down(unsigned int y_change = 1) {
        printf("\033[%dB", y_change);
        return 0;
    }    

    // print a left-aligned string at a specified x,y point (works with newlines)
    int print_left(string print_string, point start_cord) {
        set_cursor(start_cord);
        int lines = 1;
        point curr_cord = {start_cord.x, start_cord.y};
        for (int i = 0; i < print_string.size(); i++){
            // reset to next line if newline detected
            if (print_string.at(i) == '\n'){
                curr_cord.x = start_cord.x;
                curr_cord.y += 1;
                lines++;
                set_cursor(point{curr_cord.x, curr_cord.y});
                continue;
            }
            printf("%c",print_string.at(i));
        }
        return lines;
    }

    // print a right-aligned string at a specified x,y point (works with newlines)
    int print_right(string print_string, point start_cord) {
        string cut_string = "";
        int y_offset = 0;
        int x_offset = 0;
        for (int i = 0; i < print_string.size(); i++) {
            if (print_string.at(i) == '\n'){
                x_offset = cut_string.size() - 2;
                set_cursor(point{start_cord.x - x_offset, start_cord.y + y_offset});
                cout << cut_string;
                y_offset += 1;
                cut_string = "";
                continue;
            }
            cut_string += print_string.at(i);
        }
        x_offset = cut_string.size() - 2;
        set_cursor(point{start_cord.x - x_offset, start_cord.y + y_offset});
        cout << cut_string;
        return 0;
    }

    // print a center-aligned string between a specified x,y point (start_cord) to a specified length (center_len) (works with newlines)
    int print_center(string print_string, point start_cord, unsigned int center_len) {
        string cut_string = "";
        int y_offset = 0;
        int x_offset = 0;
        for (int i = 0; i < print_string.size(); i++) {
            if (print_string.at(i) == '\n'){
                x_offset = (center_len - cut_string.size()) / 2;
                set_cursor(point{start_cord.x + x_offset, start_cord.y + y_offset});
                cout << cut_string;
                y_offset += 1;
                cut_string = "";
                continue;
            }
            cut_string += print_string.at(i);
        }
        x_offset = (center_len - cut_string.size()) / 2;
        set_cursor(point{start_cord.x + x_offset, start_cord.y + y_offset});
        cout << cut_string;
        return 0;
    }

    // print a string downward starting at a speciifed x,y point
    // when it encounters a newline it will return to the top and continue printing downward
    int print_down(string print_string, point start_cord) {
        point curr_cord = {start_cord.x, start_cord.y};
        for (int i = 0; i < print_string.size(); i++){
            // reset to next line if newline detected
            if (print_string.at(i) == '\n'){
                curr_cord.y = start_cord.x;
                curr_cord.x += 1;
                set_cursor(point{curr_cord.x, curr_cord.y});
                continue;
            }
            set_cursor(curr_cord);
            cout << print_string.at(i);
            curr_cord.y += 1;
    
        }
        return 0;
    }

    // draws a line from left to right with a start_char '#', center_char '=', end_char '#'
    int draw_line_left(point start_cord, unsigned int length, char start_char = '#', char center_char = '=', char end_char = '#'){
        set_cursor(start_cord);
        cout << generate_line(length, start_char, center_char, end_char);
        return 0;
    }

    // draws a line from top to bottom with a start_char '#', center_char '|', end_char '#'
    int draw_line_down(point start_cord, unsigned int length, char start_char = '#', char center_char = '|', char end_char = '#'){
        string line = generate_line(length, start_char, center_char, end_char);
        for (int i = 0; i < line.size(); i++) {
            set_cursor(point{start_cord.x, start_cord.y + i});
            printf("%c", line.at(i));
        }
        return 0;
    }

    // draw box starting at the upper-left point given a height and length
    // 1  2
    // 4  3
    int draw_box_down(point start_cord, unsigned int length, unsigned int height, char corner1 = '/', char corner2 = '\\', char corner3 = '/', char corner4 = '\\', char vertical = '|', char horizontal = '-'){
        draw_line_left(start_cord, length, corner1, horizontal, corner2);
        draw_line_left(point{start_cord.x, start_cord.y + height}, length, corner4, horizontal, corner3);
        draw_line_down(start_cord, height, corner1, vertical, vertical);
        draw_line_down(point{start_cord.x + length, start_cord.y}, height, corner2, vertical, vertical);
        return 0;
    }

    // fills an area starting at a point (start_point) with a length and width with a char ' '
    int fill(point start_cord, unsigned int length, unsigned int height, char fill_char = ' ') {
        for (int y_offset = 0; y_offset < height; y_offset++){
            set_cursor(point{start_cord.x, start_cord.y + y_offset});
            for (int i = 0; i < length; i++){
                printf("%c", fill_char);
            }
        }
        return 0;
    }

    // takes a pointer to a text_box object and erases it and then writes it to a new location
    int move_text_box(text_box move_this, point new_point) {
        fill(move_this.start_point, move_this.length, move_this.height);
        if (move_this.type == 0) {
            print_left(move_this.text, new_point);
        }
        else if (move_this.type == 1) {
            print_right(move_this.text, point{new_point.x + move_this.length, new_point.y});
        }
        else if (move_this.type == 2) {
            print_center(move_this.text, new_point, move_this.length);
        }
        else if (move_this.type == 3) {
            print_down(move_this.text, new_point);
        }
        return 0;
    }
};
cursor_write terminal;

// Get Keyboard State
class keyboard_state {
public:
    short key_state_prev[256];
    const int winlen = 51;
    const char winkeylist[51] = { '\b', '\t', '\n', ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ';', '=', ',', '-', '.', '/', '`' , '[', '\\', ']', '\''};
    const char altkeylist[51] = { '\b', '\t', '\n', ' ', ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', '<', '_', '>', '?', '~' , '{', '|' , '}', '\"'};
    const int wincodelist[51] = { 8,    9,    13,   32,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  186, 187, 188, 189, 190, 191, 192,  219, 220,  221, 222 };
    // given a character it fines the corresponding window key code
    int find_key(char key) {
        for (int i = 0; i < winlen; i++){
            if (winkeylist[i] == key){
                return wincodelist[i];
            }
        }
        for (int i = 0; i < winlen; i++){
            if (altkeylist[i] == key){
                return wincodelist[i];
            } 
        }
        return -1;
    }

    // returns an output depending on the state of the key
    //  0 = key up, 1 = key pressed, 2 = key down
    int get_key_press(int code){
        // key press
        if (GetKeyState(code) < 0){
            if (key_state_prev[code] >= 0){
                key_state_prev[code] = -127;
                return 1; // key was just pressed
            } else {
                return 2; // key is currently pressed
            }
        }
        key_state_prev[code] = 0;
        return 0; // key is not pressed
    }

    // press enter to continue
    int wait_for_enter() {
        while (true) {
            if (get_key_press(13) == 1) {
                return 0;
            }
        }
    }

    // constructor
    keyboard_state() {
        for (int i = 0; i < 256; i++){
            key_state_prev[i] = 0;
        }
    }
};
keyboard_state keyboard;

// Stats
const string stat_name[] = {"Strength", "Endurance", "Precision", "Mental_Fortitude"};
#define Strength 0
#define Endurance 1
#define Precision 2
#define Mental_Fortitude 3

// Conditions
const string condition_name[] = {"Health", "Sanity", "Reputation", "Punchability"};
#define Health 0
#define Sanity 1
#define Reputation 2
#define Punchability 3

class entity {
private:
    //              Strength    Endurance   Precision   Mental_Fortitude
    int stats[4] = {1,          1,          1,          1};
    //                  Health  Sanity      Reputation  Punchability
    int condition[4] = {100,    50,          20,          30};
    int score = 0;
public:
    int passed_day() {
        score++;
        return 0;
    }
    int set_condition(int code, int change) {
        if (change >= 0){
            condition[code] = min(change + condition[code], 100);
        } else {
            condition[code] = max(change + condition[code], 0);
        }
        return 0;
    }

    int get_condition(int code) {
        return condition[code];
    }

    int set_stats(int code, int change) {
        if (change >= 0){
            stats[code] = min(change + stats[code], 5);
        } else {
            stats[code] = max(change + stats[code], 0);
        }
        return 0;
    }

    int get_stats(int code) {
        return stats[code];
    }

    // updates changing portions of stat window
    int update_stat_window() {
        terminal.print_center("Conditions", point{80,0}, 22);
        for (int i = 0; i < 4; i++) {
            terminal.print_left(condition_name[i] + ": " + to_string(condition[i]) + "  ", point{80,i * 2 + 1}); // prints name of condition then value, I.E: Health: 80
            terminal.print_left(generate_status_bar(condition[i]), point{80,i * 2 + 2}); // prints status bar
        }
        return 0;
    }

    int process_stat(vector<int> stat_modifiers) {
        for (int i = 0; i < stat_modifiers.size(); i += 2) {
            set_stats(stat_modifiers.at(i), stat_modifiers.at(i + 1));
        }
        return 0;
    }

    int process_condition(vector<int> process_modifiers) {
        for (int i = 0; i < process_modifiers.size(); i += 2) {
            set_condition(process_modifiers.at(i), process_modifiers.at(i + 1));
        }
        return 0;
    }

    int check_failure_conditions() {
        bool failure = false;
        // health failure
        if (condition[Health] == 0) {
            terminal.clear_screen();
            terminal.set_cursor(point{0,0});
            cout << "You collapse from exhaustion and are set to be medically discharged\n" <<
                    "Should've laid off on those peanut butter packets fatty\n\n";
            failure = true;
        }
        else if (condition[Sanity] == 0) {
            terminal.clear_screen();
            terminal.set_cursor(point{0,0});
            cout << "After a manic episode you accidentally say the magic words to your Senior DI\n" <<
                    "Now you can't even have your own shoelaces\n\n";
            failure = true;
        }
        else if (condition[Reputation] == 0) {
            terminal.clear_screen();
            terminal.set_cursor(point{0,0});
            cout << "\"Clearly you don't care about your reputation\"\nat least thats what the Senior DI said\nWell it doesn't matter now, a discharge is a discharge\n\n";
            failure = true;
        }
        else if (condition[Punchability] == 100) {
            terminal.clear_screen();
            terminal.set_cursor(point{0,0});
            cout << "Have you ever seen the first bootcamp DI scene in Jarhead?\nImagine that but 10x worse\nat least you made the local news";
            failure = true;
        }
        if (failure) {
            printf("You survived %d", score);
            if (score == 1) {
                cout << " Day\n";
            }
            else {
                cout << " Days\n";
            }
            while (true) {};
        }
        return 0;
    }

    string debug_string(){
        string outstring = "";
        // grab conditions
        for (int i = 0; i < 4; i++) {
            outstring += condition_name[i];
            outstring += ": ";
            outstring += to_string(condition[i]);
            outstring += '\n';
        }

        // grab stats
        for (int i = 0; i < 4; i++) {
            outstring += stat_name[i];
            outstring += ": ";
            outstring += to_string(stats[i]);
            outstring += '\n';
        }
        return outstring;
    }
};
entity player;

// these are the options that events have
// string option, vector<int> condition_mod, string outcome_msg
struct option {
    string option = "";
    // formatted code, change, code, change
    // I.E: Stamina, 15, Endurance -15
    vector<int> condition_mod;
    string outcome_msg;
};

class event {
public:
    string description = "";
    vector<option> options;

    int run_event() {
        unsigned int y_offset = 0;
        terminal.print_left(description, point{0,1});
        
        // prints options offsets
        for (int i = 0; i < options.size(); i++) {
            terminal.print_left(to_string(i) + ')' + options.at(i).option, point{0,3u + y_offset});
            y_offset++;
        }

        // user input loop
        bool break_loop = false;
        while (!break_loop) {
            for (unsigned int i = 0; i < options.size(); i++) {
                if (keyboard.get_key_press(48 + i)){
                    terminal.print_left(options.at(i).outcome_msg, point{0,4u + y_offset});
                    player.process_condition(options.at(i).condition_mod);
                    
                    // print conditions
                    terminal.set_cursor(point{80, 9});
                    for (int z = 0; z < options.at(i).condition_mod.size(); z += 2) {
                        cout << condition_name[options.at(i).condition_mod.at(z)] << ' ';
                        if (options.at(i).condition_mod.at(z + 1) > 0) {
                            cout << '+';
                        }
                        cout << options.at(i).condition_mod.at(z + 1) << ' ';
                    }

                    terminal.set_cursor(point{0,3u + i});
                    break_loop = true;
                    break;
                }
            }
        }

        keyboard.wait_for_enter();
        terminal.fill(point{0,0},80,15);
        player.update_stat_window();
        player.check_failure_conditions();
        player.passed_day();
        terminal.fill(point{80,9}, 50, 1); // to clear the condition changes
        terminal.set_cursor(point{0,6 + y_offset});
        return 0;
    }
};

class event_manager() {
private:
    vector<event> generic_events;
}

int main(){
    
    terminal.clear_screen();
    terminal.set_cursor(point{0,0});
    cout << "You have made the mistake of joining the Marines\n" <<
            "This is your attempt to get through bootcamp\n\n" <<
            "Press ENTER to continue\n";
    keyboard.wait_for_enter();
    terminal.clear_screen();
    player.update_stat_window();

    // Make A Rack Event
        event rack = {"You Encounter A Rack"};
        rack.options.push_back(option{" Make Your rack", {Sanity, 15, Punchability, -10, Reputation, 5}, "\"This is the worst rack I've ever seen\"\nWell at least you tried"});
        rack.options.push_back(option{" Jump off 3rd Deck", {Sanity, 25, Health, -15}, "You attempt to jump out the window but forget to open it"});
        rack.options.push_back(option{" Roll on the floor like an eel", {Sanity, 30, Punchability, 15}, "You Look Really Stupid"});
        rack.run_event();
    // PT Event
        event pft = {"Today you have your annual Physical Fitness Test"};
        pft.options.push_back(option{" Be a Tryhard", {Sanity, -20, Punchability, -10, Reputation, 10}, "You vomit halfway through the run and have to put it into your pocket\nat least you got a decent time"});
        pft.options.push_back(option{" Shitbag the run",{Sanity, 10, Punchability, 10, Reputation, -10},"Your DIs yell at you for walking, but hey what do they know about running?"});
        pft.options.push_back(option{" Try to hide in the bushes and cheat your time",{Sanity, 20, Punchability, 20, Reputation, -20},"You run and try to hide in the bushes halfway through the run\nUnfortunately you forgot that you needed to record your time so you got caught"});
        pft.run_event();
    // Chow Event
        event chow = {"You arrive at the chowhall"};
        chow.options.push_back(option{" I gotta pick the healthy options", {Sanity, -10, Health, 10}, "You eat an overcooked pork chop and salad\nAt least it isn't box chow"});
        chow.options.push_back(option{" I'm gonna eat 16 peanut butter packets", {Sanity, 10, Health, -5, Reputation, -10}, "You realize far too late that you ate too many packets\n\"Turn on the Vacuum\""});
        chow.options.push_back(option{" I earned desert", {Sanity, 20, Punchability, 10, Health, -15}, "Your DIs caught you and made you throw it away halfway through eating it\n\"Was it worth it?\""});
        chow.run_event();
    // DI Event
        event DI_encounter = {"You encounter a Rogue DI"};
        DI_encounter.options.push_back(option{"Greet the DI", {Sanity, -10, Punchability, -10, Reputation, 10}, "The DI has you clean one of their empty barracks\nNo rest for the wicked"});
        DI_encounter.options.push_back(option{"Turn The other direction to avoid them", {Sanity, 10, Punchability, 10, Reputation, -10}, "Just as you are about to clear the corner you hear \"ZERO\"\nThe DI did not think you were clever"});
        DI_encounter.options.push_back(option{"Continue Walking and Ignore the DI", {Health, -25, Punchability, 30, Reputation, -20}, "Seriously, what did you expect to happen?\n"});
        DI_encounter.run_event();
}
