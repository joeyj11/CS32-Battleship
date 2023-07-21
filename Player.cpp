#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;
void removePoint(Point p, vector<Point>& v)
{
    auto it = v.begin();
    for (; it != v.end(); )
    {
        if (it->r == p.r && it->c == p.c)
            it = v.erase(it);
        else
            it++;
    }
}

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************
//typedef AwfulPlayer HumanPlayer;
bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}


class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g) : Player(nm, g){}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack(); // this is where the human is going to give its attack so that itis the same for all classes
    virtual bool isHuman() const {return true;} // this is overriding the base classes function
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {}
    virtual void recordAttackByOpponent(Point p) {}
  private:
    Point m_lastCellAttacked;
    
};


bool HumanPlayer::placeShips(Board& b) // this is how the players place the ships on the board
{
    int r, c;
    string dir;
    
    
    cout << name() << " must place " << game().nShips() << " ships.\n";
    b.display(false); // we want to see where the ships are on this board
    for(int i = 0; i < game().nShips(); i++) //for as many ships there are in the game
    {
        
        cout << "Enter h or v for direction of " << game().shipName(i) <<  " (length " << game().shipLength(i) << "): ";
        cin >> dir;
        while (dir != "h" && dir != "v") // this is here to make sure that the input is correct

        {
            cout << "Direction must be h or v.\n";
            cout << "Enter h or v for direction of " << game().shipName(i) <<  " (length " << game().shipLength(i) << "): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> dir;
        }
        if(dir == "h")
        {
            cout << "Enter row and colum of leftmost cell (eg., 3 5): ";
            getLineWithTwoIntegers(r, c);
            while(b.placeShip(Point(r,c), i, HORIZONTAL) == false) //keep inputing until it is a correct place to attack
            {
                cout << "The ship cannot be placed there.\n";
                cout << "Enter row and colum of leftmost cell (eg., 3 5): ";
                getLineWithTwoIntegers(r, c);
            }
            
            b.placeShip(Point(r,c), i, HORIZONTAL); //once the input is correct we can place the ship
            b.display(false); // we want to be able to see the ships on the board
        }
        if(dir == "v")
        {

            cout << "Enter row and colum of topmost cell (eg., 3 5): ";
            getLineWithTwoIntegers(r, c);
            while(!b.placeShip(Point(r,c), i, VERTICAL)) //keep inputing until it is a correct place to attack
            {
                cout << "The ship cannot be placed there.\n";
                cout << "Enter row and colum of topmost cell (eg., 3 5): ";
                getLineWithTwoIntegers(r, c);
            }
            b.placeShip(Point(r,c), i, VERTICAL); //once the input is correct we can place the ship
            b.display(false); // we want to be able to see the ships on the board
        }
    }
    return true;
    
}

Point HumanPlayer::recommendAttack()
{
    int r,c; //these are going to be the cordinates that get attacked
    cout<< "Enter the row and column to attack (e.g., 3 5): ";
    cin >> r >> c;
    while (!cin.good()) // this is how we check to see if it is an int
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //found this on stack overflow to check if was int
        cout << "You must enter two integers." << endl;
        cout<< "Enter the row and column to attack (e.g., 3 5): ";
        getLineWithTwoIntegers(r, c);
    }
  
    
    return Point(r,c); // we don't do any bounds checking here the player can input whatever numbers
}




// TODO:  You need to replace this with a real class declaration and
//        implementation.

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player
{
  public:
    MediocrePlayer(string nm, const Game& g) :Player(nm, g)
    {
        for(int i = 0; i < MAXROWS; i++)
        {
            for(int j = 0; j < MAXCOLS; j++)
                m_OldAttack[i][j] = '.';
        }
                
    }
    virtual bool placeShips(Board& b);
    bool RplaceShips(Board &b, int nships, Point p, Direction d, int id, bool Backtrack, vector<Point> pships, vector<Direction> dships);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p) {}// this does nothing
    void CrossOfPoints(int r, int c); // this is going to building the cross
  private:
    char m_OldAttack[MAXROWS][MAXCOLS]; // double vector or 2d array
    vector<Point> m_Cross; // this is where the points are stored
    Point m_lastCellAttacked;
    bool m_lastShotHit = false;
    bool m_destroyed;
    int m_state = 1; //m state should always begin at one
    
};

bool MediocrePlayer::placeShips(Board& b) // this is the public interface
{
    //our recursive function will start at (0,0)
    // and horizonatly
    int amount =0 ; // to make sure that our function doesn't call over 50 times
    bool valid = false;
    while(!valid && amount < 50)
    {
        b.block(); // this blocks have the board for us to place our ships
        valid = RplaceShips(b, game().nShips(),  Point(0,0), HORIZONTAL, 0, false, {}, {}); // this is the function that atemps to place
        amount++;
        b.unblock(); // unblocking the the board
    
    }
    return valid; // if it every works it will return true
}

bool MediocrePlayer::RplaceShips(Board &b, int nships, Point p, Direction d, int id, bool Backtrack,vector<Point> pships, vector<Direction> dships) //pships will keep track of the ships that have actually been placed, dships keeps track of their direction
{
    //b.display(false);
    if(nships == 0 ) //this is our base case
        return true;
    if(p.c + 1 > game().cols()) // if the recursive call hits the end of the row
    {
        p.c = 0; //go to the first column
        p.r++; // of the next row
    }
    if(p.r + 1 > game().rows() && p.c == 0) // if we go through all the rows we are going to need to start back tracking
    {
        Backtrack = true; // now we are going to back track
    }
    if(pships.size() ==0 && Backtrack) // this is preventing a bad access or else it will try and point from an empty array
        return false;
    
    bool valid;
    if(!Backtrack)
    {
            valid = b.placeShip(p, id, d); // if we can place the ship here then valid would be true
        
        if(valid) // if this worked we need to keep track of how it worked
        {
            pships.push_back(p); // keep track of the id that it worked on
            dships.push_back(d); // in was direction did it work
            return RplaceShips(b, nships-1, Point(0,0), HORIZONTAL, id+1, false, pships, dships); //recursive call
        }
        else // if it didn't work we need to try in in the other direction
        {
            if(d == HORIZONTAL)
            {
                d = VERTICAL;
                return RplaceShips(b, nships, p, VERTICAL, id, Backtrack, pships, dships); // this is the recursive call
            }
            else // if d is already vertical and that didn't work
                return RplaceShips(b, nships, Point(p.r, p.c+1), HORIZONTAL, id, Backtrack, pships, dships);
                // try again moving to the right
        }
    }
    else // only hits this if back track is true
    {
        Point backPoint = pships.back();
        int row = backPoint.r; //get the row of the last ship added
        int col = backPoint.c; //get the col of the last ship added
        pships.pop_back(); //"Removing the last ship"
        Direction dir = dships.back();
        dships.pop_back();
        b.unplaceShip(Point(row,col), id-1, dir); // this is the part that actually removes the ship
        // the -1 is necesary bc the recursive call calls +1 after it is added
        
        if(dir == HORIZONTAL)
            return RplaceShips(b, nships+1, Point(row,col), VERTICAL, id-1, false, pships, dships); // if it was horizontal try it the other dir
        else
            return RplaceShips(b, nships+1, Point(row,col+1), HORIZONTAL, id-1, false, pships, dships); // it it was already vertical move to the next column
        
    }
    return true;
}

void MediocrePlayer::CrossOfPoints(int r, int c)
{
    m_Cross.clear(); // make sure there are no points we we start
    
    for(int i = 1; i <=4; i++)
    {
        int d = i;
        if (r-d >= 0 && m_OldAttack[r-d][c] == '.') // this is moing the point up
               m_Cross.push_back(Point(r-d, c));
        if (r+d <= game().rows()-1 && m_OldAttack[r+d][c] == '.') // down
            m_Cross.push_back(Point(r+d, c));
        if (c-d >= 0 && m_OldAttack[r][c-d] == '.') // left
            m_Cross.push_back(Point(r, c-d));
        if (c+d <= game().cols()-1 && m_OldAttack[r][c+d] == '.') //right
            m_Cross.push_back(Point(r, c+d));
    }
}

Point MediocrePlayer::recommendAttack()
{
    int r = m_lastCellAttacked.r;
    int c = m_lastCellAttacked.c;
    
    if(m_state == 1 ) // if the last shot wasn't hit we need to attack the a randone point
    {
        bool isValid = false; // we need to make sure we don't generate a place that we already hit
        while(isValid == false)
        {
            isValid = true; // will be true unless we prove it otherwise
            int r = randInt(game().rows()); // this just generates a rand place to attaack
            int c = randInt(game().cols());
                if(m_OldAttack[r][c] == 'X')
                {
                    isValid = false;
                }
            if(isValid == true)
                return(Point(r,c)); // the recommended attack is going to be the random point
        }
    }
    else //THIS IS STATE 2
    {
        
        int rand = randInt(m_Cross.size()); // this determines what point that we grab in the array
        Point attack(m_Cross[rand].r, m_Cross[rand].c); // this will be our point to attack
        m_Cross.erase(m_Cross.begin() + rand); //get rid of the point that we just used
        if(m_Cross.empty())
            m_state = 1;
        return attack;
        
    }
    
   // cerr << "OOPS I HIT THE BOTTOM\n";
    return(Point(r,c)); // THIS SHOULD NEVER BE HIT BUT THE FUNCTION MUST RETURN IN ALL PATHS
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    
    m_OldAttack[p.r][p.c] = 'X'; // mark this so that we know if it has been attacked // don't care about hit or miss
    
    if(!validShot)
        cerr << "Should not be invalid shot\n";
    
    // RULES: if we want to go to state two we must hit a ship however the ship cannot be destroyed
    // only if m state is equal to one are we going to update the cordinate hit otherwise the algorithm would move around
    if(shotHit && shipDestroyed == false && m_state == 1) // this is where we will create the new points
    {
        
        m_lastShotHit = true;
        CrossOfPoints(p.r, p.c); // creates the limited range from the hit point
        m_lastCellAttacked = p;
        m_state = 2;
        m_destroyed = false;
    }
    if(shotHit && shipDestroyed == false) // this once virtually has no effect because we stay in state two and don't pass a new point
    {
        m_lastShotHit = true;
        m_state = 2;
        m_destroyed = false;
    }
    // If we are in State 2 are need to leave then we must destroy a ship
    if(shotHit && shipDestroyed == true) // a ship must be destory to leave
    {
        m_lastShotHit = true;
        m_lastCellAttacked = p;
        m_state = 1;
        m_Cross.clear();
        m_destroyed = false;
    }
    
    if(!shotHit) // this does nothing
    {
        m_lastShotHit = false;
    }
    
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player
{
  public:
    GoodPlayer(string nm, const Game& g) :Player(nm, g)
    {
        for(int i = 0; i < MAXROWS; i++)
        {
            for(int j = 0; j < MAXCOLS; j++)
                m_OldAttack[i][j] = '.';
        }
    }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId);
    //virtual void recordAttackByOpponent(Point p);
    void CrossOfPoints(int r, int c); // this is going to building the cross(small one here)
  private:
    char m_OldAttack[MAXROWS][MAXCOLS]; // double vector or 2d array
    stack<Point> old_hits;
    vector<Point> m_Cross; // this is where the points are stored
    Point m_lastCellAttacked;
    Point m_secondLastHit;
    Point m_refrence;
    vector<Point> hitInThree;
    //int m_LastShotResult;
    bool m_lastShotHit = false;
    bool m_destroyed;
    int m_state = 1; //m state should always begin at one
    bool arePoints;
    int m_count = 0; // this counts how many times the ships were placed with distance from each other
    int m_boardarea = game().rows() * game().cols();
    bool State3Full = false;
};

bool GoodPlayer::placeShips(Board& b)
{
    bool valid = false;
    int id = 0;
    int Ships = game().nShips();
    //we want to place this ships randomly
    while(Ships > 0)
    {
        int rrow = randInt(game().rows()); //creating a random place to place the ships
        int rcol = randInt(game().cols());
        valid = b.placeShip(Point(rrow, rcol),id, HORIZONTAL); // lets first place it hor
        if(!valid)
            valid = b.placeShip(Point(rrow, rcol), id, VERTICAL); // then lets try vertically
        if(valid)
        {
            Ships--;
            id++;
        }
    }
    return true;
}

void GoodPlayer::CrossOfPoints(int r, int c)
{
    arePoints = false; // this makes sure that we actually fill with points if not we stay in stay one
    
    for(int i = 1; i <2; i++) // we are just going to use the cross once in each direction
    {
        
        bool isValid = true;
        int d = i;

            if (r-d >= 0 && m_OldAttack[r-d][c] == '.') // this is moing the point up
            {
                for(int i = 0; i< m_Cross.size(); i++)
                {
                    if(m_Cross[i].r == r-d && m_Cross[i].c == c)
                        isValid = false;
                        
                }
                if(isValid == true)
                    m_Cross.push_back(Point(r-d, c));
            }
        isValid = true;
            if (r+d <= game().rows()-1 && m_OldAttack[r+d][c] == '.') // down
            {
                for(int i = 0; i< m_Cross.size(); i++)
                {
                    if(m_Cross[i].r == r+d && m_Cross[i].c == c)
                    isValid = false;
                        
                }
                if(isValid == true)
                    m_Cross.push_back(Point(r+d, c));
            }
        isValid = true;
            if (c-d >= 0 && m_OldAttack[r][c-d] == '.')
            {
                for(int i = 0; i< m_Cross.size(); i++)
                {
                    if(m_Cross[i].r == r && m_Cross[i].c == c-d)
                        isValid = false;
                        
                }
                if(isValid == true)
                    m_Cross.push_back(Point(r, c-d));
            }
        isValid = true;
        if (c+d <= game().cols()-1 && m_OldAttack[r][c+d] == '.') //right
        {
            for(int i = 0; i< m_Cross.size(); i++)
            {
                if(m_Cross[i].r == r && m_Cross[i].c == c+d)
                    isValid = false;
                    
            }
            if(isValid == true)
                m_Cross.push_back(Point(r, c+d));
        }
        
    
        else
        {
        if(r-d-1 >=0 && m_OldAttack[r-d-1][c] == '.')
        {
        if (r-d >= 0 && m_OldAttack[r-d][c] == '.') // this is moing the point up
            m_Cross.push_back(Point(r-d, c));
        }
        if(r+d-1 <= game().rows()-1 && m_OldAttack[r+d-1][c] == '.')
        {
        if (r+d <= game().rows()-1 && m_OldAttack[r+d][c] == '.') // down
            m_Cross.push_back(Point(r+d, c));
        }
        if (c-d-1 >= 0 && m_OldAttack[r][c-d-1] == '.')
        {
        if (c-d >= 0 && m_OldAttack[r][c-d] == '.') // left
            m_Cross.push_back(Point(r, c-d));
        }
        if (c+d-1 <= game().cols()-1 && m_OldAttack[r][c+d-1] == '.')
        {
            if (c+d <= game().cols()-1 && m_OldAttack[r][c+d] == '.') //right
                m_Cross.push_back(Point(r, c+d));
        }
        }
      
    }
    if(!m_Cross.empty())
    {
        arePoints = true;
        
    }
        
}

Point GoodPlayer::recommendAttack()
{
    int r;
    int c;
    
    if(m_state == 1 ) // if the last shot wasn't hit we need to attack in the pattern
    {
        int c = randInt(game().cols());
                int r = randInt(game().rows());
                if (m_count < (m_boardarea/1.85)) {
                    while (m_OldAttack[r][c] != '.' || (c % 2 != 0 && r % 2 == 0 || c % 2 == 0 && r % 2 != 0)) {
                        r = randInt(game().rows());
                        c = randInt(game().cols());
                    }
                }
                else {
                    while (m_OldAttack[r][c] != '.') {
                        r = randInt(game().rows());
                        c = randInt(game().cols());
                    }
                }
                //[r][c] = 1;
                m_count++;
                m_lastCellAttacked = Point(r, c); // 3, 5
                return (Point(r, c));
    }

    else if (m_state == 2)
     {
         m_count++;
         //cout << "I'M IN STATE 2\n";
         
        
         bool isValid = false;
         
         while(isValid == false)
        {
            if(!m_Cross.empty())
            {
        // int rand = randInt(m_Cross.size()); // this determines what point that we grab in the array
         Point attack(m_Cross[0].r, m_Cross[0].c); // this will be our point to attack
         if(m_OldAttack[attack.r][attack.c] == '.')
             isValid = true;
         m_Cross.erase(m_Cross.cbegin()); //get rid of the point that we just used
         if(m_Cross.empty())
             m_state = 1;
         if(isValid == true)
             return attack;
            }
            else
            {
                //cout<< "NOOOOOO\n";
\
                 c = randInt(game().cols());
                         r = randInt(game().rows());
                        if (m_count < (m_boardarea/1.85)) {
                            while (m_OldAttack[r][c] != '.' || (c % 2 != 0 && r % 2 == 0 || c % 2 == 0 && r % 2 != 0)) {
                                r = randInt(game().rows());
                                c = randInt(game().cols());
                            }
                        }
                        else {
                            while (m_OldAttack[r][c] != '.') {
                                r = randInt(game().rows());
                                c = randInt(game().cols());
                            }
                        }
                        m_count++;
                        m_lastCellAttacked = Point(r, c); // 3, 5
                        return (Point(r, c));
                  }
            }

         }
         
     else if(m_state == 3) // if we hit a shot after we are in state 2
     {
         m_count++;
         //cout << "I'M IN STATE 3\n";
         Direction dir;
         Point lastHit = old_hits.top();
         
         if(lastHit.r == m_secondLastHit.r)
             dir = HORIZONTAL;
         else
             dir = VERTICAL;
         

             if(dir == HORIZONTAL)
             {
                 //cout << "Horizontal\n";
                 if( m_refrence.c+1 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +1] == '.') //check to make sure it has been hit and on the board
                     return Point(m_refrence.r,m_refrence.c+1);
                 if(m_refrence.c -1 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -1] == '.')
                     return Point(m_refrence.r,m_refrence.c-1);
                 
                 if( m_refrence.c+1 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +1] != 'o')
                 {
                     if(m_refrence.c +2 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +2] == '.')
                         return Point(m_refrence.r,m_refrence.c+2);
                     
                     if( m_refrence.c+2 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +2] != 'o')
                     {
                         if( m_refrence.c +3 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +3] == '.')
                             return Point(m_refrence.r,m_refrence.c+3);
                     }
                     if( m_refrence.c+3 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +3] != 'o')
                     {
                         if( m_refrence.c +4 < game().cols() && m_OldAttack[m_refrence.r][m_refrence.c +4] == '.')
                             return Point(m_refrence.r,m_refrence.c+4);
                     }
                 }
                 
                 if(m_refrence.c-1 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -1] != 'o')
                 {
                     if(m_refrence.c -2 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -2] == '.')
                         return Point(m_refrence.r,m_refrence.c-2);
                 
                     if(m_refrence.c-2 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -2] != 'o')
                     {
                         if( m_refrence.c -3 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -3] == '.')
                             return Point(m_refrence.r,m_refrence.c-3);
                     }
                     
                     if(m_refrence.c-3 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -3] != 'o')
                     {
                         if( m_refrence.c -4 >= 0 && m_OldAttack[m_refrence.r][m_refrence.c -4] == '.')
                             return Point(m_refrence.r,m_refrence.c-4);
                     }
                 }

             }
             if(dir == VERTICAL)
             {
                 //cout << "Vertical\n";
                 if( m_refrence.r+1 < game().rows() && m_OldAttack[m_refrence.r+1][m_refrence.c] == '.') // checking to make sure it hast been hit
                 {
                     //cout << "out of bounds\n";
                     return Point(m_refrence.r+1,m_refrence.c);
                 }
                 //cout<< "HERE!\n";
                 if(m_refrence.r -1 >= 0 && m_OldAttack[m_refrence.r-1][m_refrence.c] == '.')
                     return Point(m_refrence.r-1,m_refrence.c);
                 //cout<< "here?\n";
                 if(m_refrence.r +1 < game().rows() && m_OldAttack[m_refrence.r+1][m_refrence.c] != 'o')
                 {
                     if(m_refrence.r +2 < game().rows() && m_OldAttack[m_refrence.r+2][m_refrence.c] == '.')
                         return Point(m_refrence.r+2,m_refrence.c);
                     //cout << "here!!\n";
                     if(m_refrence.r +2 < game().rows() && m_OldAttack[m_refrence.r+2][m_refrence.c] != 'o')
                     {
                         if(m_refrence.r +3 < game().rows() && m_OldAttack[m_refrence.r+3][m_refrence.c] == '.')
                             return Point(m_refrence.r+3,m_refrence.c);
                     }
                     
                     if(m_refrence.r +3 < game().rows() && m_OldAttack[m_refrence.r+3][m_refrence.c] != 'o')
                     {
                         if(m_refrence.r +4 < game().rows() && m_OldAttack[m_refrence.r+4][m_refrence.c] == '.')
                             return Point(m_refrence.r+4,m_refrence.c);
                     }
                 }
                 //cout << "leaky\n";
                 if(m_refrence.r -1 >=0 && m_OldAttack[m_refrence.r-1][m_refrence.c] != 'o')
                 {
                     //cout << "down here\n";
                     if(m_refrence.r -2 >= 0 && m_OldAttack[m_refrence.r-2][m_refrence.c] == '.')
                         return Point(m_refrence.r-2,m_refrence.c);
                     if(m_refrence.r -2 >=0 && m_OldAttack[m_refrence.r-2][m_refrence.c] != 'o')
                     {
                         
                         if(m_refrence.r -3 >= 0 && m_OldAttack[m_refrence.r-3][m_refrence.c] == '.')
                             return Point(m_refrence.r-3,m_refrence.c);
                     }
                     
                     if(m_refrence.r -3 >=0 && m_OldAttack[m_refrence.r-3][m_refrence.c] != 'o')
                     {
                         if(m_refrence.r -4 >= 0 && m_OldAttack[m_refrence.r-4][m_refrence.c] == '.')
                             return Point(m_refrence.r-4,m_refrence.c);
                     }
     
             }
                 
        
         if(!m_Cross.empty()) //now attack the remainting point in the stack
         {
             //cout << "I leaked from 3\n";
             Point Attack(m_Cross[0].r, m_Cross[0].c);
             if(m_OldAttack[m_Cross[0].r][m_Cross[0].c] == '.')
             {
                 m_Cross.erase(m_Cross.begin());
                 if(m_Cross.empty())
                    m_state = 1;
                 else
                     m_state = 2;
                 return(Attack);
             }
             else
                 m_Cross.erase(m_Cross.begin());

         }
         
     }
     }
    //cout<< "NOOOOOO\n";
     //m_state = 1;
     c = randInt(game().cols());
             r = randInt(game().rows());
            if (m_count < (m_boardarea / 2)) {
                while (m_OldAttack[r][c] != '.' || (c % 2 != 0 && r % 2 == 0 || c % 2 == 0 && r % 2 != 0)) {
                    r = randInt(game().rows());
                    c = randInt(game().cols());
                }
            }
            else {
                while (m_OldAttack[r][c] != '.') {
                    r = randInt(game().rows());
                    c = randInt(game().cols());
                }
            }
            //[r][c] = 1;
            m_count++;
            m_lastCellAttacked = Point(r, c); // 3, 5
            return (Point(r, c));
      
    
    
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    
    
    if(shotHit)
    {
        m_OldAttack[p.r][p.c] = 'X'; // mark this so that we know if it has been attacked //
        if(!old_hits.empty()) // if we are in 2 then we need to find the correct dir for the next attack
            m_secondLastHit = old_hits.top();
        old_hits.push(p); //new top is the last hit®
    }
    else
        m_OldAttack[p.r][p.c] = 'o'; // mark this so that we know if it has been attacked //
    
   
    
  //  if(!validShot)
     //   cerr << "Should not be invalid shot\n";
    
    // RULES: if we want to go to state two we must hit a ship however the ship cannot be destroyed
    // only if m state is equal to one are we going to update the cordinate hit otherwise the algorithm would move around
    if(shotHit && shipDestroyed == false)
    {
        m_lastShotHit = true;
        if(m_state == 3)
        {
            hitInThree.push_back(p);
            CrossOfPoints(p.r,p.c);
        }
        if(m_state == 2)
        {
            m_state = 3; // new type of state we can limit the direction
            m_refrence = m_secondLastHit;
            CrossOfPoints(p.r, p.c);
        }
        
        m_destroyed = false;
    }
    
    if(shotHit && shipDestroyed == false && m_state == 1) // this is where we will create the new points
    {
        
        m_lastShotHit = true;
        CrossOfPoints(p.r, p.c); // creates the limited range from the hit point
        if(arePoints)
            m_state = 2;
        else
            m_state = 1;
        m_lastCellAttacked = p;
        m_destroyed = false;
    }
   
    // If we are in State 2 are need to leave then we must destroy a ship
    if(shotHit && shipDestroyed == true) // a ship must be destory to leave
    { //   THOUGHT DO WE WANT TO LEAVE STATE THREE IF HAVE HIT LESS TILES THAN THE SHIP THAT WAS DESTROYED COME BACK HERE AND TRY AND MAKE IT THAT IT GOES BACK TO THE ORGINAL CROSS CREATED IN STATE TWO SO WE DESTROY BOTH SHIPS THAT WERE HIT
        m_lastShotHit = true;
        m_lastCellAttacked = p;
        //m_state = 1;
        m_destroyed = true;
        
        
        if(m_state == 3)
        {
            hitInThree.push_back(p);
            CrossOfPoints(p.r, p.c);
        }
        
        int lenDestroyed  = game().shipLength(shipId); // keep track of if we hit more shots than ship
        
        
        if(lenDestroyed < hitInThree.size()+ 2) //plus one for the one that we hit in 2
        {
            if(!m_Cross.empty())
            {
                m_state = 2;
                old_hits.push(m_refrence); // this line puts the old refrence back on top
            }
        }
        else
        {
            m_Cross.clear();
            m_state = 1;
        }

        hitInThree.clear();
       
        
        
         
    }
    
    if(!shotHit)
    {
        m_lastShotHit = false;
    }
    
}



//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}


