#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char board[MAXROWS][MAXCOLS]; // this is the game board
    vector<int> placedShipsID;
    vector<int> destroyedShipsID; //to check for destroyed ship just walk the whole array looking for its symbol(in attack)
    vector<int> remainingShipId;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    for(int i = 0; i < m_game.rows(); i++)
    {
        for(int j = 0; j < m_game.cols(); j++)
            board[i][j] = '.'; //intilizing the board to the "empty value"
    }
}

void BoardImpl::clear()
{
    for(int i = 0; i < m_game.rows(); i++)
    {
        for(int j = 0; j < m_game.cols(); j++)
            board[i][j] = '.'; //intilizing the board to the "empty value"
    }
}

void BoardImpl::block()
{
    
    int numberCells = m_game.rows() * m_game.cols();
    int block = numberCells/2;
    int blocked = 0; // the number we have actually blocked (a counter)
    
    while(blocked != block) // will run until half of the cells are blocked
    {
        int randRow = randInt(m_game.rows());
        int randCol = randInt(m_game.cols());
        
        if(board[randRow][randCol] == '.')
        {
            board[randRow][randCol] = '+';
            blocked++; // will only count up if we block the tile
        }
    }
    assert(blocked == block);
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(board[r][c] == '+')
                board[r][c] = '.'; // set all the tiles with + back to .
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId < 0 || shipId > m_game.nShips() ) //make sure that the ship id is valid
        return false;
    
    for(int i=0; i < placedShipsID.size(); i++) //this is to make sure we aren't placing a ship that has already been placed
    {
        if(placedShipsID[i] == shipId) // this indicates that the ship would have been already placed
            return false;
    }
    
    int len = m_game.shipLength(shipId); // this is how many chars on the board we are going to mark
    if(topOrLeft.r + 1 > m_game.rows() || topOrLeft.c +1 >m_game.cols() || topOrLeft.r < 0 || topOrLeft.c < 0) //making sure the point is not off the board
        return false;
    if(dir == HORIZONTAL) //what to do if the ship is align horizontally
    {
        for(int i = 0; i < len; i++) //first we need to walk throught once and then start placing
        {
            if(topOrLeft.c + len > m_game.cols()) // make sure the rows go exceed moving to the right
                return false;
            if(board[topOrLeft.r][topOrLeft.c+i] != '.') // the row stays the same for the horizontal ship and the column increases to the left
            {
                return false;
            }
        }
        for(int i = 0; i < len; i++) // now we go throug and place the ship now we know that it is correct
        {
            board[topOrLeft.r][topOrLeft.c+i] = m_game.shipSymbol(shipId);
        }
        placedShipsID.push_back(shipId); // keep track of the ships that have already been placed
    }
    
    if(dir == VERTICAL) //what to do if the ship is align Vertically
    {
        int r = topOrLeft.r; //for readability
        int c = topOrLeft.c;
        for(int i = 0; i < len; i++) //first we need to make sure that all of the spots where we are going to be placing are open
        {
            if(r + len > m_game.rows()) // make sure the rows don't go off the board going down
                return false;
            if(board[r+i][c] != '.') // row should increase
            {
                return false;
            }
        }
        for(int i = 0; i < len; i++) // now we go throug and place the ship now we know that it is correct
        {
            board[topOrLeft.r+i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        placedShipsID.push_back(shipId); // keep track of the ships that have already been placed
    }
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    int len = m_game.shipLength(shipId);
    if(shipId < 0 || shipId > m_game.nShips()) //make sure that the ship id is valid
        return false;
    
    for(int i=0; i < placedShipsID.size(); i++) //this is to make sure the ship we are removing has already been placed
    {
        if(placedShipsID[i] == shipId)
        {
            placedShipsID.erase(placedShipsID.begin() + i);
            break;
        }
        if(i == placedShipsID.size() - 1) //if it makes it this far without finding the id return false
            return false;
    }
    
    if(dir == HORIZONTAL)
    {
        for(int i=0; i < len; i++)
        {
            if(board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId)) // this checks to make sure the loction really contains the ship
                return false;
            else
                board[topOrLeft.r][topOrLeft.c + i] = '.'; //set the loction back to the period
        }
    }
    
    if(dir == VERTICAL)
    {
        for(int i=0; i < len; i++)
        {
            if(board[topOrLeft.r+i][topOrLeft.c] != m_game.shipSymbol(shipId)) // this checks to make sure the loction really contains the ship
                return false;
            else
            {
                board[topOrLeft.r+i][topOrLeft.c] = '.'; // set the location back to the .
        }
    }
    }
    return true;
}
void BoardImpl::display(bool shotsOnly) const //THIS FUNCTION IS UNFISHED //DO THIS NOW
{
    //FIRST LINE IS "  " AND THE NUMBERS 0-9
    cout << "  ";
    for(int i = 0; i < m_game.cols(); i++)
        cout << i;
    cout << endl;
    
    //FOR THE REMAINING ROWS
    for(int i=0; i < m_game.rows(); i++) //go throught the array row by row
    {
        cout << i << " "; //print the row number and the space
        for(int j=0; j < m_game.cols(); j++) //print the data member of every column
        {
            if(shotsOnly)
            {
                if(board[i][j] != 'o' && board[i][j] != 'X') //these are the cases were it would be a ship
                    cout<< '.';
                else
                    cout << board[i][j];
            }
            else
                cout << board[i][j]; //just show the board if not shotsonly
        }
        cout << endl; // new line for the new row
        
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    shotHit = false;
    shipDestroyed = false;
    if(p.r+1> m_game.rows() || p.c+1> m_game.cols() || p.r < 0 || p.c <0) //checking to make sure that the attack was on the board
        return false;
    if (board[p.r][p.c] == 'o' || board[p.r][p.c] == 'X') // attack is not succesful if we hit a spot we already attacked before
       {
           shipId = -1;
           return false;
       }
    if(board[p.r][p.c] != '.') // if is not equal to . excluding above condidition then it was a hit
    {
        for(int i = 0; i < placedShipsID.size(); i++)
        {
            if(board[p.r][p.c] == m_game.shipSymbol(placedShipsID[i])) // this is meant to figure out which ship we just hit
            {
                shipId = i; // the ID is the int of the position of the array its in
                shotHit = true;
                break;
            }
        }
        board[p.r][p.c] = 'X'; //mark the ship as hit
        shotHit = true;
        
        for(int r = 0; r < m_game.rows(); r++) // this is checking to see if there are any symbols of this ship left to determine if it was destroyed or not
        {
            for(int c = 0; c < m_game.cols(); c++)
            {
                if(board[r][c] == m_game.shipSymbol(shipId))
                {
                    shipDestroyed = false; //if there is any of the same symbol on the board still than the ship is not detroyed
                    return true; // this will catch and return if there are any symbols left
                }
                    
            }
        }
        destroyedShipsID.push_back(shipId); // if there is no symbols left then the ship is destroyed and we add it to the destroyed ship array
        shipDestroyed = true;
        return true;
    }
    
    if(board[p.r][p.c] == '.')
        board[p.r][p.c] = 'o';
    
    return true;
    
}

bool BoardImpl::allShipsDestroyed() const
{
    if(placedShipsID.size() == destroyedShipsID.size()) // this checks to see if the size of the place ships array is the same as the destroyed ship array
        return true; // if they are the same then all the ships have been destroyed and we can return true
    else
        return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g) // create a board that refers to a game
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear() //clears the board so it can be populated with ships
{
    m_impl->clear();
}

void Board::block() //blocks RC/2 randomly cohsen position on the board
{
    return m_impl->block();
}

void Board::unblock() //this function will be used by MediocrePlayers are the placing ships on the board
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{

    //FOR ALL THE SHIPS IN THE ARRAY PLACE A SHIP 
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
