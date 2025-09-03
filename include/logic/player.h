#ifndef PLAYER_H

#define PLAYER_H



class Player {
    public:
    int hp;
    int score;
    int x, y;
    int attackPower; 
    
    Player();
    void takeDamage(int damage);
    void addScore(int points);
    void reset();

    ~Player();


};

#endif //PLAYER_H