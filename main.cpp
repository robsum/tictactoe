#include <iostream>
#include <array>
#include <stdexcept>
#include <random>

constexpr int BOARD_SIZE = 3;

class Board {
    public:
        enum Field { Empty, X, O };
        using Mat3x3 = std::array<std::array<Field, BOARD_SIZE>, BOARD_SIZE>;
        static const char* chars;// = {'-', 'X', 'O'};
        static const Field* fields;// = { Field::Empty, Field::X, Field::O };

        static Field charToField(char value) {
            //int index = std::distance(chars, std::find(chars, chars+2, value));
            //std::cout << "Returning" << index;
            int index;
            switch (value) {
                case 'X':
                case 'x':
                    index = 1;
                    break;
                case 'O':
                case 'o':
                    index = 2;
                    break;
                default:
                    throw std::invalid_argument("Char out of range.");
            }
            //std::cout << "Value " << value << " converted to field " << fields[index] << std::endl;
            return fields[index];

        }
        static char fieldToChar(Field field) {
            int index;
            switch (field) {
                case Field::Empty:
                    index = 0;
                    break;
                case Field::X:
                    index = 1;
                    break;
                case Field::O:
                    index = 2;
                    break;
                default:
                    throw std::invalid_argument("Field out of range.");
            }
            //std::cout << "Field " << field << " converted to value " << chars[index] << std::endl;
            return chars[index];
        }
    private:
        Mat3x3 _board;
    public:
        Mat3x3 getBoard() { return _board; };
        Board() {
            Mat3x3 mat {
                std::array{ Field::Empty, Field::Empty, Field::Empty },
                std::array{ Field::Empty, Field::Empty, Field::Empty },
                std::array{ Field::Empty, Field::Empty, Field::Empty }
            };
            _board = mat;
        };
        bool setField(int x, int y, Field value) {
            if (x >= BOARD_SIZE || y > BOARD_SIZE) { throw std::invalid_argument("Field out of range."); return false; }
            _board[x][y] = value;
            return true;
        };
        
        void drawBoard() {
            bool columns_printed = false;
            int row_number = 0;
            for (auto& row : _board) {
                if (!columns_printed) {
                    std::cout << ' ';
                    for (int i = 0; i < BOARD_SIZE; ++i) {
                        std::cout << i;
                    }
                    std::cout << std::endl;
                    columns_printed = true;
                }
                std::cout << row_number;
                for (Field& column : row) {
                    std::cout << fieldToChar(column);
                }
                std::cout << std::endl;
                row_number++;
            }
        };
        bool isFieldEmpty(int x, int y) {
            return _board[x][y] == Field::Empty;
        }
        bool isEndGameForField(Field field) {
            if (field == Field::Empty) { return false; }
            for (auto& row : _board) {
                if (row[0] == field && row[1] == field && row[2] == field) {
                    return true;
                }
            }
            if (_board[1][1] == field && ( (_board[0][0] == field && _board[2][2] == field) || (_board[0][2] == field && _board[2][0] == field) )) {
                return true;
            }
            for (int i = 0; i < BOARD_SIZE; ++i) {
                if (_board[0][i] == field && _board[1][i] == field && _board[2][i] == field) {
                    return true;
                }
            }
            return false;
        }
        bool isEndGame() {
            bool allFieldsAreFilled = true;
            for (auto& row: _board) {
                for(auto& field : row) {
                    if (field == Field::Empty) { allFieldsAreFilled = false; }
                }
            }
            return allFieldsAreFilled && (isEndGameForField(Field::X) || isEndGameForField(Field::O));
        }
        
};

static const char c[] = {'-', 'X', 'O'};
const char* Board::chars = c;
static const Board::Field f[] = { Board::Field::Empty, Board::Field::X, Board::Field::O };
const Board::Field* Board::fields = f;

int main(int argc, char* argv[]) {
    Board board = Board();

    std::cout << "Tic tac toe game! Board:" << std::endl;
    board.drawBoard();

    std::cout << "Please enter which row and column you want to insert your X: (ex. 0,0) (q,q to exit)" << std::endl;

    char x, y, value, separator;
    
    while (std::cin >> x >> separator >> y ) {
        if (x == 'q' || y == 'q' || separator == 'q') {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        //std::cout << "Setting position (" << x << ',' << y << ") to " << value << '(' << Board::charToField(value) << ')' << std::endl;
        if (board.isFieldEmpty(x - '0', y - '0')) {
            board.setField(x - '0', y - '0', Board::Field::X);
        } else { std::cout << "Field is already filled." << std::endl; continue; }

        if (board.isEndGame()) { board.drawBoard(); break; }

        int x_ai, y_ai;
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 2); // define the range

        x_ai = distr(gen);
        y_ai = distr(gen);
        while (!board.isFieldEmpty(x_ai, y_ai)) {
            x_ai = distr(gen);
            y_ai = distr(gen);
        }
        board.setField(x_ai, y_ai, Board::Field::O);

        board.drawBoard();
        if (board.isEndGame()) { break; }
    }

    if (board.isEndGameForField(Board::Field::X)) { std::cout << "Congratulations, you won!" << std::endl; }
    else if (board.isEndGameForField(Board::Field::O)) { std::cout << "Too bad, you have lost!" << std::endl; }
    else { std::cout << "It's a tie!" << std::endl; }

    std::cout << "Thank you for playing." << std::endl;

    return EXIT_SUCCESS;
}