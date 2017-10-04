#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct players{
    int hand{0};                //Valgte å gå for structs istedenfor klasser fordi eg bare skulle bruke det til å holde styr på data
    int handSoft{0};
    int money{100};             //har lagt inn både handSoft og AI her så eg kan passe heile structen til cardValCalc()
    bool AI{false};
};

void restock(int (&deck)[52]);
void shuffle(int cards, int (&deck)[52]);
int draw(int (&deck)[52]);
int printCard(int card);
int cardValueCalc(int val, players* currentPlayer);



int main()
{
    srand(time(NULL));      //Starter med å seede rand, duh.

    int deck[52];           //valgte å gå med deck istedenfor å bare velge kort tilfeldig for å få det mer autentisk uten
                            //å ha for komplisert tilfeldighets algoritme
    players dealer;
    dealer.AI = true;
    players player;

    bool playAgain;

    do                          //Gått for do-loops i mesteparten av programmet grunnet at det meste skal kjøres minst ein gang
    {
        restock(deck);
        shuffle(52, deck);
        player.hand = 0;
        dealer.hand = 0;

        bool drawAnother;

        cout << "Players wallet: " << player.money << "$" << endl
             << "House: " << dealer.money << "$" << endl << endl;       //printer kor masse penger alle har

        do
        {
            int tempVal = cardValueCalc(printCard(draw(deck)), &player);    //draw trekker eit kort og returnerer eit tall fra 1-52 til printCard
                                                                            //printCard sorterer då etter verdi og suit på kortet for å så returnere
            player.hand += tempVal;                                         //verdien videre til cardValueCalc
                                                                            //cardValueCalc regner då ut verdiene på face cardsene og ess

            if(player.handSoft != 0)            //legger verdien av kortet som er trukket inn i tempVal sånn at den kan legges
                player.handSoft += tempVal;     //inn i både player.hand og player.handSoft
            if(player.handSoft != 0 && player.handSoft <= 21)
                cout << endl << "Total hand value = soft " << player.handSoft << endl;
            else                                                                        //vist spilleren har trukket ess vil den då ha ein soft hand
                cout << endl << "Total hand value = " << player.hand << endl;           //Vist soft hand er mindre en 21 vil man uansett spille etter
            if(player.hand < 21)                                                        //soft hand
            {
                cout << "Do you want to draw another card? 0 for no, 1 for yes" << endl;    //sjekker her om hand er lavere en 21
                cin >> drawAnother;                                                         //Ingen vits å trekke kort om den er 21 eller høgere
            }

        } while(drawAnother && player.hand < 21);           //fortsetter å trekke kort så lenge spilleren vil og hand value er under 21

        if(player.handSoft <= 21 && player.handSoft > player.hand)
            player.hand = player.handSoft;          //velger den beste verdien mellom player.hand og player.handSoft
                                                    //og passer på at den verdien ligger i player.hand etter dette punktet
        cout << "End hand value = " << player.hand << endl;     //printer End hand value så det blir lettere å se kva verdi man avsluttet med

        if(player.hand <= 21)           //sjekker om spilleren er innenfor til å fortsette. Vist verdien er over 21 vil spilleren
        {                               //automatisk tape 10$
            int bet;

            do
            {
                cout << "How much do you bet?" << endl;
                cin >> bet;

                if(bet > player.money)
                    cout << "You do not have that much money!" << endl;     //spilleren vedder penger og sjekker at verdiene går
                else if(bet > dealer.money)
                    cout << "You can not bet more than what your opponent has left!" << endl;
                else if(bet < 10)
                    cout << "You need to bet more than 10$" << endl;

            } while(bet > player.money || bet > dealer.money || bet < 10);      //looper til spilleren har funnet ein respektabel verdi å vedde

            do
            {
                dealer.hand += cardValueCalc(printCard(draw(deck)), &dealer);       //sender cardValueCalc sin return value direkte til dealer.hand
                                                                                    //fordi dealer ikkje bruker handSoft
                cout << endl << "Total hand value = " << dealer.hand << endl;
            } while(dealer.hand < player.hand && dealer.hand < 21);

            if(dealer.hand > 21)
            {
                cout << "The player wins!" << endl
                     << "Recived " << bet << " from the house." << endl;
                player.money += bet;
                dealer.money -= bet;
            }                                                   //sjekker om kem som vant
            else if(dealer.hand == player.hand)
                cout << "Draw!" << endl;
            else if(player.hand < dealer.hand)
            {
                cout << "Player lost!" << endl;
                dealer.money += bet;
                player.money -= bet;
            }
        }
        else
        {
            cout << "The house wins!" << endl
                 << "You lost 10$" << endl;
            player.money -= 10;
            dealer.money += 10;
        }

        cout << "Play again? 0 for no, 1 for yes" << endl;          //spør om spilleren til spille igjen og greier

        cin >> playAgain;

        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";     //clearer skjermen
    }while(playAgain && player.money >= 10 && dealer.money >= 10);      //sjekker at alt er på plass for å starte på nytt

    if(player.money < 10)
        cout << "You are out of money!" << endl
             << "You lose!" << endl;        //forteller grunnen til korfor spillet avsluttet vist spilleren velger å spille igjen
    if(dealer.money < 10)                               //men enten spilleren eller huset er tomt for penger
        cout << "The house is out of money!" << endl
             << "You win!" << endl;

}

void restock(int (&deck)[52])           //fyller opp deck med tall fra 1-52 som representerer kortene
{
    for(int i = 0; i < 52; i++)
    {
        deck[i] = i + 1;
    }
}

void shuffle(int cards, int (&deck)[52])    //stokker om alle verdiene sånn at man kan trekke tilfeldige kort seinere
{
    int tempDeck[cards];
    int r;

    for(int i = cards; i > 0; i--)
    {
        r = rand()%i;       //velger eit tall mellom 0 og i - 1
                            //i står for antall "ustokkede" kort igjen i kortstokken
        tempDeck[i - 1] = deck[r];      //setter verdien av det tilfeldig valgte kortet i deck inn i i - 1 i tempDeck
                                        //fylles opp
        if(r != (i - 1))
        {
            deck[r] = deck[i - 1];          //flytter øverste kortet i deck til deck[r] som nett blei satt inn i tempDeck
        }                                   //ellers blir ikkje det kortet tatt med i neste omgang siden verdien går ned med 1
    }

    for(int i = 0; i < cards; i++)
    {
        deck[i] = tempDeck[i];      //pakker ut tempDeck og legger alle kortene inn igjen i deck
    }

}

int draw(int (&deck)[52])       //trekker øverste kortet i kortstokken
{

    int tempCard;
    int i{-1};

    do
    {
        i++;
        tempCard = deck[i];     //kjører loopen fram til det første kortet som ikkje er trukket allerede
    } while(deck[i] == 0);

    deck[i] = 0;        //setter det trukkede kortets verdi til 0 for å vise at det er trukket

    return tempCard;        //returnerer kortet
}

int printCard(int card)         //printer kortet på konsollen og sorterer verdien
{
    int suit = 1;

    if(39 < card)
    {
        card -= 39;
        suit = 4;
    }
    else if(26 < card)
    {
        card -= 26;                 //sjekker først om kortet er 1-13, 14-26, 27-39 eller 40-52 og assigner 1-4 til suit ettersom
        suit = 3;                   //kva suit kortet er
    }
    else if(13 < card)
    {
        card -= 13;
        suit = 2;
    }

    switch(card)
    {
    case 1:
        cout << "Ace ";
        break;
    case 2:
        cout << "Two ";
        break;
    case 3:
        cout << "Three ";
        break;
    case 4:
        cout << "Four ";
        break;
    case 5:
        cout << "Five ";
        break;
    case 6:
        cout << "Six ";
        break;
    case 7:
        cout << "Seven ";                   //første delen printer ut verdien på kortet
        break;
    case 8:
        cout << "Eight ";
        break;
    case 9:
        cout << "Nine ";
        break;
    case 10:
        cout << "Ten ";
        break;
    case 11:
        cout << "Jack ";
        break;
    case 12:
        cout << "Queen ";
        break;
    case 13:
        cout << "King ";
        break;
    }

    cout << "of ";          //alle kort har "of" i midten så den er plassert i mellom

    switch(suit)
    {
    case 1:
        cout << "Hearts";
        break;
    case 2:
        cout << "Clubs";
        break;                      //printer suiten på kortet
    case 3:
        cout << "Diamonds";
        break;
    case 4:
        cout << "Spades";
        break;
    }

    return card;        //returner verdien til kortet så man ikkje trenger å sortere igjen i cardValueCalc
}

int cardValueCalc(int val, players* currentPlayer)      //regner ut om kortet er verdt 1-11
{
    switch(val)
    {
    case 1:
        if(currentPlayer->AI)
        {
            if(currentPlayer->hand <= 10)
                return 11;                  //veldig simpel AI, er hand 10 eller mindre er ess lik 11, vist ikkje er den lik 1
            else
                return 1;
        }
        else
        {
            if(currentPlayer->hand <= 10)
                currentPlayer->handSoft = currentPlayer->hand + 10;     //spilleren har litt mer valg og har derfor ein handSoft
                                                                        //vist hand er 10 eller mindre vil handSoft få verdien til
            return 1;                                                   //hand + 10 siden den siste eineren returneres og legges til
        }                                                               //handSoft utenfor funksjonen
    case 11:
        return 10;
    case 12:
        return 10;              //returnerer 10 dersom kortet er verdt 11-13 siden facecardsene har verdien 10
    case 13:
        return 10;
    default:
        return val;
    }
}

//test
