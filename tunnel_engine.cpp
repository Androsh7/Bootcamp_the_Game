#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#include <windows.h>

using namespace std;

#define window_x 50
#define window_y 15

struct point{
    int x, y;
};

class window_class{
private:
    char window[window_y][window_x];
public:
    // prints borders around the screen
    int create_border(char border_vertical = '|', char border_horizontal = '-', char border_corner1 = '/', char border_corner2 = '\\'){
        // print top and bottom row
        for (int row = 0; row < window_y; row += window_y - 1){
            for (int x = 1; x < window_x - 1; x++){
                window[row][x] = border_horizontal;
            }
        }

        // print left and right columns
        for (int column = 0; column < window_x; column += window_x - 1){
            for (int y = 1; y < window_y - 1; y++){
                window[y][column] = border_vertical;
            }
        }

        // print upper left and right corners
        window[0][0] = border_corner1;
        window[0][window_x - 1] = border_corner2;

        // print lower left and right corners
        window[window_y - 1][0] = border_corner2;
        window[window_y - 1][window_x - 1] = border_corner1;
    
        return 0;
    }
    
    int set_cord(point set_point, char fill_char){
        // bounds check
        if (set_point.x > window_x || set_point.x < 0 || set_point.y > window_y || set_point.y < 0){
            cerr << "ERROR IN SET_CORD COMMAND, OUT OF BOUNDS REQUEST FOR (" << set_point.x << ',' << set_point.y << ")\n";
            return 1;
        }
        else {
            window[set_point.y][set_point.x] = fill_char;
        }
        return 0;
    }

    char get_cord(point get_point){
        // bounds check
        if (get_point.x > window_x || get_point.x < 0 || get_point.y > window_y || get_point.y < 0){
            cerr << "ERROR IN GET_CORD COMMAND, OUT OF BOUNDS REQUEST FOR (" << get_point.x << ',' << get_point.y << ")\n";
            return '\0';
        }
        return window[get_point.y][get_point.x];
    }

    // fill the window with the specified char (can specify an offset and a rectangular size)
    int fill(point fill_offset = point{0,0}, int fill_x_size = window_x, int fill_y_size = window_y, char fill_char = ' '){
        for (int y = fill_offset.y; y < fill_y_size; y++){
            for (int x = fill_offset.x; x < fill_x_size; x++){
                // bounds check
                if (x < fill_offset.x || x > fill_x_size || y < fill_offset.y || y > fill_y_size){
                    cerr << "ERROR IN FILL COMMAND, OUT OF BOUNDS REQUEST FOR (" << x << ',' << y << ")\n";
                    return 1;
                }
                else {
                    window[y][x] = fill_char;
                }
            }
        }
        return 0;
   }

    // fills everything except the specified area
    int reverse_fill(point upperleft_boundary, point lowerright_boundary, point fill_offset = point{0,0}, int fill_x_size = window_x, int fill_y_size = window_y, char fill_char = ' '){
        for (int y = fill_offset.y; y < fill_y_size; y++){
            for (int x = fill_offset.x; x < fill_x_size; x++){
                // cancel fill if within exclusion area
                if (upperleft_boundary.y <= y && lowerright_boundary.y >= y && upperleft_boundary.x <= x && lowerright_boundary.x >= x){
                    continue;
                }
                // bounds check
                if (x > window_x || x < 0 || y > window_y || y < 0){
                    cerr << "ERROR IN FILL COMMAND, OUT OF BOUNDS REQUEST FOR (" << x << ',' << y << ")\n";
                    return 1;
                }
                else {
                    window[y][x] = fill_char;
                }
            }
        }
        return 0;
   }

   // prints a left-aligned string starting at a specifed cord
   void add_string_left(string instring, point offset){
        point curr{offset.x, offset.y};
        for (int i = 0; i < instring.size(); i++){
            if (instring.at(i) == '\n'){
                curr.y += 1;
                curr.x = offset.x;
                continue;
            }
            window[curr.y][curr.x] = instring.at(i);
            curr.x += 1;
        } 
   }
   
   // prints a right-aligned string start at a specified cord
   int add_string_right(string instring, point offset){
        string curr_string = "";
        point curr{offset.x, offset.y};
        for (int i = 0; i < instring.size(); i++){
            if (instring.at(i) == '\n'){
                for (int z = curr_string.size() - 1; z >= 0; z--){
                    window[curr.y][curr.x] = curr_string.at(z);
                    curr.x -= 1;
                }
                curr.x = offset.x;
                curr.y += 1;
                curr_string = "";
                continue;
            }
            curr_string += instring.at(i);
        }
        return 0;
   }

    // only allows for drawing lines left-to-right
   int draw_horizontal_line(point start_point, int x_len, char start_char = '-', char line_char = '-', char end_char = '-'){
        set_cord(point{start_point.x,start_point.y},start_char);
        for (int x = 1; x < x_len; x++){
            set_cord(point{start_point.x + x,start_point.y},line_char);
        }
        set_cord(point{start_point.x + x_len - 1, start_point.y}, end_char);
        return 0;
   }

    // only allows for drawing lines downward
   int draw_vertical_line(point start_point, int y_len, char start_char = '-', char line_char = '-', char end_char = '-'){
        set_cord(point{start_point.x,start_point.y},start_char);
        for (int y = 1; y < y_len; y++){
            set_cord(point{start_point.x, start_point.y + y},line_char);
        }
        set_cord(point{start_point.x, start_point.y + y_len - 1}, end_char);
        return 0;
   }

   void print_window(bool debug_mode = false){
        if (debug_mode){
            cout << "   0";
            for (int i = 0; i < window_x / 10; i++){
                cout << "123456789-";
            }
            cout << "\n";
        }
        for (int y = 0; y < window_y; y++){
            if (debug_mode) printf("%2u", y);
            for (int x = 0; x < window_x; x++){
                cout << window[y][x];
            }
            cout << "\n";
        }
   }

   window_class() {
        // pads the window array with spaces
        for (int y = 0; y < window_y; y++){
            for (int x = 0; x < window_x; x++){
                window[y][x] = ' ';
            }
        }

        // create the window border
        create_border();
   }
};

void clear_screen(){
    cout << "\033[2J\033[1;1H";
}

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

int main() {
    // build stat window
    window_class stat_window;
    stat_window.add_string_right("Test_String\nhello\ntest\n123",point{11,1});
    stat_window.draw_horizontal_line(point{0,5}, 24, '#', '-', '\\');
    stat_window.draw_vertical_line(point{23,5}, 10, '\\', '|', '#');
    int fatigue = 54;
    stat_window.add_string_left("Fatigue: " + to_string(fatigue) + '%',point{6,6});
    stat_window.add_string_left(generate_status_bar(fatigue),point{1,7});
    stat_window.print_window(true);
}
