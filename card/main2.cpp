#include<iostream>
#include<iomanip>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

struct Card{
	int rank,suit;
	bool operator<(const struct Card & r) const{ //sort関数を利用する場合に判定式となる
		if(rank != r.rank) return (rank > r.rank);
		if(suit != r.suit) return (suit < r.suit);
		return 0;
	}
};

void make_deck(stack<Card> &deck){
	Card work;
	for(int i=0;i<52;i++){
		work.rank=i%13+1;
		work.suit=i%4;
		deck.push(work);
	}
}

void shuffle_cards(stack<Card> &deck,int n){
	int a,b;
	Card cards[52],work;
	srand(time(NULL));
	for(int i=0;i<52;i++){
		cards[i] = deck.top();
		deck.pop();
	}
	for(int i=0;i<100000;i++){
		a = rand() % n;
		b = rand() % n;
		work = cards[a];
		cards[a] = cards[b];
		cards[b] = work;
	}
	for(int i=0;i<52;i++) deck.push(cards[i]);
}

void dist_card(stack<Card> &deck,Card player[4][13]){ //各プレイヤーにカードを配る。
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			player[i][k] = deck.top();
			deck.pop();
			cout << player[i][k].rank << " ";//<< "s:" << player[i][k].suit << " ";
		}
		cout << endl;
	}
}

void player_cards_init(Card player_cards[][13]){
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			player_cards[i][k].rank = 0;
		}
	}
}

int cnt_cards(Card player_cards[][13],int player_number){ //プレイヤーの手札を数える。
	int i=0;
	while(player_cards[player_number][i].rank > 0){
		i++;
		//cout << player_cards[player_number][i].rank << " ";
	}
	//cout << i << endl;;
	return i;
}

void print_player1_hand(Card player_cards[][13]){
	static const string suit_mark = "SHDC";
	static const string rank_mark[14] = {"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
	int cnt = cnt_cards(player_cards,0);
	for(int i=0;i<cnt;i++){
		//cout << rank_mark[player_cards[0][i].rank];
	}
	//cout << endl;
}

void set_player_cards(Card player_cards[][13],int player_number,queue<Card> &work){
	int i=0;
	while(! work.empty()){
		player_cards[player_number][i] = work.front();
		work.pop();
		i++;
	}
}

//void exclusion_player_cards(Card player_cards[][13],int player_number,Card  exclusion_card,queue<Card> &work){
//	int max_cards = cnt_cards(player_cards,player_number);
//	for(int i=0;i<max_cards;i++){
//		if((player_cards[player_number][i].rank != exclusion_card.rank) &&
//		  (player_cards[player_number][i].suit != exclusion_card.suit)) work.push(player_cards[player_number][i]);
//	}
//	set_player_cards(player_cards,player_number);
//}

int set_seven(bool field[][13],Card player_cards[][13]){
	queue<Card> work;
	int i,k,dia7_owner;
	for(i=0;i<4;i++){
		field[i][6] = true;
		for(k=0;k<13;k++){
			if(player_cards[i][k].rank != 7){ //7をplayer_cardsから除外するための前処理。
				work.push(player_cards[i][k]);
			}else if(player_cards[i][k].suit == 2){
				dia7_owner = i;
				//cout << "set_seven:" << i << " " << player_cards[i][k].suit << endl;
			}
		}
		set_player_cards(player_cards,i,work);
	}
	return dia7_owner;
}

void game(Card player_cards[][13],bool field[][13],int seven){
	print_player1_hand(player_cards);
}

int main(){
	int seven_dia_player;
	bool cards_field[4][13];
	Card player_cards[4][13];
	stack<Card> deck;
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++) cards_field[i][k] = false;
	}

	make_deck(deck);
	//shuffle_cards(deck,52);
	dist_card(deck,player_cards);
	seven_dia_player = set_seven(cards_field,player_cards);
	//cout << "dia:" << seven_dia_player << endl;

	game(player_cards,cards_field,seven_dia_player);
	return 0;
}
