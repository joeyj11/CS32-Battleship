#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const; // this tells us how many ships are in the game
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows;
    int m_cols;
    int m_ships = 0;
    
    class ship // this is where we store all of the ship data
    {
    public:
        ship(int length, char symbol, string name) : m_len(length), m_marker(symbol), m_name(name) {}
        int length() const {return m_len;}
        char marker() const {return m_marker;}
        string name() const {return m_name;}
        int id() const { return m_id;}
        bool valid() const {return m_valid;}
        void setId(int IDnumber) {m_id = IDnumber;}
    private:
        int m_len; //length of the ship
        char m_marker; // what is its symbol on the board
        string m_name; // name of shipt
        int m_id; // how are we going to id the ship
        Direction m_dir = HORIZONTAL; // what is its orientation
        bool m_valid = true; // is this ship valid
        
    };
    
    std::vector<ship> ships; //this is how we are going to keep track of all of the ships in the game
    
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : m_rows(nRows), m_cols(nCols)
{} // the bounds checking is already done in Game

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const //return true if the point is on the game board
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const //return a random point on the game board
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    ship s = ship(length, symbol, name);
    if(s.valid())
    {
        m_ships++; //add one to the total number of ships in the game
        ships.push_back(s); // add the new ship to the array of ships
                            // IMPORTANT THE ID NUMBER CORRESPONDS TO ITS POSITION IN THE ARRAYx
        s.setId(m_ships-1); // set the id to the number of ships -1
        
        return true;
    }
    else {return false;}
}

int GameImpl::nShips() const
{
    return m_ships;
}

int GameImpl::shipLength(int shipId) const
{
    
    return ships[shipId].length(); //get the ship at this index and get the lenght
}

char GameImpl::shipSymbol(int shipId) const
{
    return ships[shipId].marker(); //get the ship at this index and get the char
}

string GameImpl::shipName(int shipId) const
{
    return ships[shipId].name();  // get the ship at this index and get the string
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(p1->placeShips(b1) == false)
    {
        //cerr << "Couldn't Place all ships";
        return nullptr;
    } // make sure that we can place ships
    if(p2->placeShips(b2) == false)
    {
        //cerr << "Couldn't Place all ships";
        return nullptr;} // place the ships
    
    
    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) //game goes until all ships are gone
    {
        //***********************************************************************************
        //PLAYER 1
        bool shotHit, shipDestroyed; // these are so that we can use these to keep track of what happned in the boolattack() function
        int shipId =0; // the attack function will change these vales if there is actually a hit
        bool shotsonly;
        if(p1->isHuman()) //makes sure that human play can only see shots and hits
        {
            shotsonly = true;
            shouldPause = true; // this should always be true for human players?
        }
        else {shotsonly = false;}
        
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":\n"<< endl;
        b2.display(shotsonly);
        
        Point attackHere = p1->recommendAttack(); // we need to get where to attack from the recomend function
        int row = attackHere.r; // row of where we attacked
        int col = attackHere.c; //col of where we attacked
        if(b2.attack(attackHere, shotHit, shipDestroyed, shipId)) //what are all the outcome of an attack
        {
            
            if(shotHit) //if the shot hit a ship
            {
                if(shipDestroyed) // if we hit a ship and destroy it display this
                {
                    cout << p1->name() << " attacked (" << row << "," << col << ") and destroyed the " << ships[shipId].name() <<  ", resulting in:\n";
                    b2.display(shotsonly);
                }
                else // if we hit somthing but there the ship is not destroyed
                {
                    cout << p1->name() << " attacked (" << row << "," << col << ") and hit something, resulting in:\n";
                    b2.display(shotsonly);
                }
            }
            if(shotHit == false) // if the shot did not hit a ship
            {
                cout << p1->name() << " attacked (" << row << "," << col << ") and missed, resulting in:\n";
                b2.display(shotsonly);
            }
    
        }
        else
        {
            cout << p1->name() << " wasted a shot at (" << row << " " << col << ").\n";
            
        }
        //it must be valid if we made it this far
        p1->recordAttackResult(Point(row, col), true, shotHit, shipDestroyed, shipId); // this is recording the result of the attack
        
        if(b2.allShipsDestroyed()) //this is the win condition
        {
            cout << p1->name() << " wins!\n";
            return p1; // if p1 destroys all of the ships on b2
        }
        
        if (shouldPause) //if human player it will pause
            waitForEnter();
            
        
    //***********************************************************************************
    //PLAYER 2
    bool shotHit2, shipDestroyed2; // these are so that we can use these to keep track of what happned in the boolattack() function
    int shipId2 = 0; // the attack function will change these vales if there is actually a hit
    bool shotsonly2;
    if(p2->isHuman()) //makes sure that human play can only see shots and hits
          shotsonly2 = true;
    else {shotsonly2 = false;}
    
    cout << p2->name() << "'s turn. Board for " << p1->name() << ":\n"<< endl;
    b1.display(shotsonly2);
    
    Point attackHere2 = p2->recommendAttack(); // we need to get where to attack from the recomend function
    int row2 = attackHere2.r; // row of where we attacked
    int col2 = attackHere2.c; //col of where we attacked
    if(b1.attack(attackHere2, shotHit2, shipDestroyed2, shipId2)) //what are all the outcome of an attack
    {
        
        if(shotHit2) //if the shot hit a ship
        {
            if(shipDestroyed2) // if we hit a ship and destroy it display this
            {
                cout << p2->name() << " attacked (" << row2 << "," << col2 << ") and destroyed the " << ships[shipId2].name() <<  ", resulting in:\n";
                b1.display(shotsonly2);
            }
            else // if we hit somthing but there the ship is not destroyed
            {
                cout << p2->name() << " attacked (" << row2 << "," << col2 << ") and hit something, resulting in:\n";
                b1.display(shotsonly2);
            }
        }
        if(shotHit2 == false) // if the shot did not hit a ship
        {
            cout << p2->name() << " attacked (" << row2<< "," << col2 << ") and missed, resulting in:\n";
            b1.display(shotsonly2);
        }
       
    }
    else
    {
        cout << p2->name() << " wasted a shot at (" << row2 << " " << col2 << ").\n";
        b1.display(shotsonly2);
 
    }
        
        //it must be valid if we made it this far
        p2->recordAttackResult(Point(row2, col2), true, shotHit2, shipDestroyed2, shipId2); // this is recording the result of the attack
        
        if(b1.allShipsDestroyed()) //this is the win condition
        {
            cout << p2->name() << " wins!\n";
            return p2; // if p2 destroys all of the ships on b1
        }
        
    if (shouldPause) //should wait if a human player
    {
        waitForEnter();
        if(p2->isHuman())
            cin.ignore(10000, '\n');
    }
        


    
    
   
}
    return nullptr;  // This compiles but may not be correct
}
//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols) // this is a constructor for game that takes a row and col
{
    if (nRows < 1  ||  nRows > MAXROWS) // checking the bounds of the row
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS) //checking the bounds of the column
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols); // this is the actual implementation of the game
                                         // pointer to o game implemenation
}

Game::~Game()
{
    delete m_impl; // deletes the implementation of the game
}

int Game::rows() const
{
    return m_impl->rows(); //get rows
}

int Game::cols() const
{
    return m_impl->cols(); //get columns
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p); //checking the bounds of the point
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint(); //just choose a random point
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1) //check the bounds of the length
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols()) //make sure that it can be on the board
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol)) //make sure the symbol makes sense
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o') // cannot use these symbols for the ship
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const //how many ships are there
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const //what is the length of the ship
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const //what is the symbol of the ship
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const //what is the name of the ship
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this); // this is pointer to the game
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

