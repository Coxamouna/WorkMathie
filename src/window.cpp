#include "window.hpp"
#define INPUT_MAX 2
#define NUM_FRAMES  2   // Number of frames (rectangles) for the button sprite texture

// Function to draw centered text
void DrawTextCentered(const char *text, Vector2 position, int fontSize, Color color) {
    // Measure the text to get its width and height
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 0);

    // Calculate the position to center the text around itself
    Vector2 textPosition = {
        (float)(position.x - textSize.x / 2.0),
        (float)(position.y - textSize.y / 2.0)
    };

    float textSpacing = 5;

    // Draw the text
    DrawTextEx(font, text, textPosition, fontSize, textSpacing, color);

}

// Part 1: Manipulate the string - Calculating the number of hours worked, rounding the number to 2 decimal place and turning it back into a string
std::string hoursStr(std::string inputStartHH, std::string inputStartMM, std::string inputEndHH, std::string inputEndMM) {
    float duration = (calculateDuration(strToInt(inputStartHH), strToInt(inputStartMM), strToInt(inputEndHH), strToInt(inputEndMM)));
    std::stringstream hours;
    hours << std::fixed << std::setprecision(2) << duration;
    return hours.str();
}

//Part 2: Manipulate the string - Calculate the total money amount worked and turn it into a string
std::string moneyStr(float duration) {
    float wage = (duration * PAY);
    std::stringstream money;
    money << std::fixed << std::setprecision(2) << wage;
    return money.str();
}

// String to Integer
int strToInt(std::string str) {
    return std::stof(str);
}

// Integer to String
std::string intToString(int num) {
    return std::to_string(num);
}

// Convert to Decimal
float convertToDecimal(int hours, int minutes) {
    return hours + minutes / 60.0;
}

// Duration
float calculateDuration(int startHH, int startMM, int endHH, int endMM) {
    if ((startHH > endHH) || (startHH == endHH && startMM > endMM)) {
        return std::abs(convertToDecimal((24 + endHH), endMM) - convertToDecimal((startHH), startMM));
    }
    else {
        return std::abs((convertToDecimal(endHH, endMM)) - (convertToDecimal(startHH, startMM)));
    }
}

// Initialize
void Init() {
    InitWindow(SCREENWIDTH, SCREENHEIGHT, TITLE.c_str());
    
    Texture2D button = LoadTexture("resources/button.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    Rectangle btnBounds = {(float)(SCREENWIDTH/2.0) - (float)(button.width/2.0), (float)(SCREENHEIGHT/2.0) - (float)(button.height/NUM_FRAMES/2.0) + 135, (float)button.width, frameHeight };

    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false;         // Button action should be activated

    std::string inputStartHH(INPUT_MAX, '0');
    std::string inputStartMM(INPUT_MAX, '0');
    std::string inputEndHH(INPUT_MAX, '0');
    std::string inputEndMM(INPUT_MAX, '0');

    int indexStartHH = 0;
    int indexStartMM = 0;
    int indexEndHH = 0;
    int indexEndMM = 0;

    Rectangle startHH = {SCREENWIDTH/2.0 - 100, 110, 55, 45};
    Rectangle startMM = {SCREENWIDTH/2.0 + 40,  110, 55, 45};
    Rectangle endHH   = {SCREENWIDTH/2.0 -100,  260, 55, 45};
    Rectangle endMM   = {SCREENWIDTH/2.0 + 40,  260, 55, 45};

    Vector2 mousePoint = {0.0, 0.0};

    bool mouseOnStartHH = false;
    bool mouseOnStartMM = false;
    bool mouseOnEndHH = false;
    bool mouseOnEndMM = false;

    SetTargetFPS(FPS);

// Main loop
    // Detect window close button or ESC key
    while (!WindowShouldClose()) {
        mousePoint = GetMousePosition();

        // Check if the mouse is on the start hour box        
        if (CheckCollisionPointRec(mousePoint, startHH)) {
            mouseOnStartHH = true;
        } else {
            mouseOnStartHH = false;
        }
        // Check if the mouse is on the start minute box 
        if (CheckCollisionPointRec(mousePoint, startMM)) {
            mouseOnStartMM = true;
        } else{
            mouseOnStartMM = false;
        }
        // Check if the mouse is on the end hour box 
        if (CheckCollisionPointRec(mousePoint,endHH)) {
            mouseOnEndHH = true;
        } else {
            mouseOnEndHH = false;
        }
        // Check if the mouse is on the end minute box 
        if (CheckCollisionPointRec(mousePoint,endMM)) {
            mouseOnEndMM = true;
        } else {
            mouseOnEndMM = false;
        }

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else {
            btnState = 0;
        }

        if (btnAction)
        {
            uploadFile();
            
            btnAction = false;

        } 

         // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;

    // Start Hour Box
        if (mouseOnStartHH) {
        // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();

        // Check if more chars have been pressed
            while (key > 0) {
                // NOTE: Only allow keys in range [0, 9]
                if ((key >= 48) && (key <= 57) && (indexStartHH < INPUT_MAX)) {    
                    // NOTE: For hours, at the first index, only allow keys in range [0, 2]
                    if ((indexStartHH == 0 && key > 50)) {
                        inputStartHH[++indexStartHH] = static_cast<char>(key);
                    }
                    if (indexStartHH == 1 && inputStartHH[0] == 50 && key >= 52) {
                        key = GetCharPressed();
                        continue;
                    }
                    inputStartHH[indexStartHH++] = static_cast<char>(key);
                }
                key = GetCharPressed();  // Check next character in the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (indexStartHH > 0) {
                    inputStartHH[--indexStartHH] = '0';
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Start Minute Box
        if (mouseOnStartMM) {
        // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
        // Check if more chars have been pressed
            while (key > 0) {
                // NOTE: Only allow keys in range [0, 9]
                if ((key >= 48) && (key <= 57) && (indexStartMM < INPUT_MAX)) {
                    if (indexStartMM == 0 && key >= 54) {
                        key = GetCharPressed();
                        continue;
                    }
                    inputStartMM[indexStartMM++] = static_cast<char>(key);
                }
                key = GetCharPressed(); // Check next character in the Q
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (indexStartMM > 0) {
                    inputStartMM[--indexStartMM] = '0';
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

    // End Hour Box
        if (mouseOnEndHH) {
        // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();

        // Check if more chars have been pressed
            while (key > 0) {
                // NOTE: Only allow keys in range [0, 9]
                if ((key >= 48) && (key <= 57) && (indexEndHH < INPUT_MAX)) {    
                    // NOTE: For hours, at the first index, only allow keys in range [0, 2]
                    if ((indexEndHH == 0 && key > 50)) {
                        inputEndHH[++indexEndHH] = static_cast<char>(key);
                    }
                    if (indexEndHH == 1 && inputEndHH[0] == 50 && key >= 52) {
                        key = GetCharPressed();
                        continue;
                    }
                    inputEndHH[indexEndHH++] = static_cast<char>(key);
                }
                key = GetCharPressed();  // Check next character in the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (indexEndHH > 0) {
                    inputEndHH[--indexEndHH] = '0';
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // End Minute Box
        if (mouseOnEndMM) {
        // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
        // Check if more chars have been pressed
            while (key > 0) {
                // NOTE: Only allow keys in range [0, 9]
                if ((key >= 48) && (key <= 57) && (indexEndMM < INPUT_MAX)) {
                    if (indexEndMM == 0 && key >= 54) {
                        key = GetCharPressed();
                        continue;
                    }
                    inputEndMM[indexEndMM++] = static_cast<char>(key);
                }
                key = GetCharPressed(); // Check next character in the Q
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (indexEndMM > 0) {
                    inputEndMM[--indexEndMM] = '0';
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

    // Draw
        BeginDrawing();
        
        ClearBackground(BACKGROUND);
        DrawText("CLOCK-IN", (int)startHH.x, (int)startHH.y - 50, 35, SYSTEMTEXT);
        
        DrawRectangleRec(startHH, BOXCOL);
        if (mouseOnStartHH) {
            DrawRectangleLines((int)startHH.x, (int)startHH.y, (int)startHH.width, (int)startHH.height, BOXLINEMOUSE);
        } else {
            DrawRectangleLines((int)startHH.x, (int)startHH.y, (int)startHH.width, (int)startHH.height, BOXLINEDEFAULT);
        }
        DrawText(inputStartHH.c_str(), (int)startHH.x + 5, (int)startHH.y + 8, 40, USERTEXT);

        DrawText(":", (int)(startHH.x + startMM.x) / 2.0 + 25, (int)(startHH.y + startMM.y) / 2.0, 45, SYSTEMTEXT);

        DrawRectangleRec(startMM, BOXCOL);
        if (mouseOnStartMM) {
            DrawRectangleLines((int)startMM.x, (int)startMM.y, (int)startMM.width, (int)startMM.height, BOXLINEMOUSE);
        } else {
            DrawRectangleLines((int)startMM.x, (int)startMM.y, (int)startMM.width, (int)startMM.height, BOXLINEDEFAULT);
        }
        DrawText(inputStartMM.c_str(), (int)startMM.x + 5, (int)startMM.y + 8, 40, USERTEXT);

        DrawText("CLOCK-OUT", (int)endHH.x, (int)endHH.y - 50, 35, SYSTEMTEXT);

        DrawRectangleRec(endHH, BOXCOL);
        if (mouseOnEndHH) {
            DrawRectangleLines((int)endHH.x, (int)endHH.y, (int)endHH.width, (int)endHH.height, BOXLINEMOUSE);
        } else {
            DrawRectangleLines((int)endHH.x, (int)endHH.y, (int)endHH.width, (int)endHH.height, BOXLINEDEFAULT);
        }
        DrawText(inputEndHH.c_str(), (int)endHH.x + 5, (int)endHH.y + 8, 40, USERTEXT);

        DrawText(":", (int)(endHH.x + endMM.x) / 2.0 + 25, (int)(endHH.y + endMM.y) / 2.0, 45, SYSTEMTEXT);

        DrawRectangleRec(endMM, BOXCOL);
        if (mouseOnEndMM) {
            DrawRectangleLines((int)endMM.x, (int)endMM.y, (int)endMM.width, (int)endMM.height, BOXLINEMOUSE);
        } else {
            DrawRectangleLines((int)endMM.x, (int)endMM.y, (int)endMM.width, (int)endMM.height, BOXLINEDEFAULT);
        }
        DrawText(inputEndMM.c_str(), (int)endMM.x + 5, (int)endMM.y + 8, 40, USERTEXT);

        std::string total = hoursStr(inputStartHH, inputStartMM, inputEndHH, inputEndMM) + '/' + "\u00A3" + moneyStr(calculateDuration(strToInt(inputStartHH), strToInt(inputStartMM), strToInt(inputEndHH), strToInt(inputEndMM))); // Pound sign in UTF8 encoding
        DrawTextCentered(total.c_str(), (Vector2){btnBounds.x + 53, btnBounds.y - 40}, 37, SYSTEMTEXT);
        
        DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame

        EndDrawing();
    }
    // De-initialize
    CloseWindow();
    
}