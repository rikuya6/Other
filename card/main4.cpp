#include<iostream>
#include<iomanip>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

const string suit_mark = "SHDC";
const string rank_mark[14] = {"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};

struct Card{
	int rank,suit;
	bool operator<(const struct Card & r) const{ //sort関数を利用する場合に判定式となる
		if(rank != r.rank) return (rank > r.rank);
		if(suit != r.suit) return (suit < r.suit);
		return 0;
	}
};

struct Coordinate{
	int point,row,col;
	bool operator<(const struct Coordinate & r) const{
		if(point > r.point) return 0;
		else return 1;
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
			//cout << player[i][k].rank << " " << player[i][k].suit << " ";
		}
		//cout << endl;
	}
}

void player_cards_init(Card player_cards[][13]){
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			player_cards[i][k].rank = 0;
		}
	}
}

//int cnt_cards(Card player_cards[][13],int player_number){ //プレイヤーの手札を数える。
//	int i=0;
//	while(player_cards[player_number][i].rank > 0){
//		i++;
//		//cout << player_cards[player_number][i].rank << " ";
//	}
//	//cout << i << endl;;
//	return i;
//}

void print_player1_hand(Card player_cards[][13],int hands_cnt[]){
	//cout << hands_cnt[0] << endl;
	cout << "プレイヤーのカード" << endl;
	for(int i=0;i<hands_cnt[0];i++){
		cout << rank_mark[player_cards[0][i].rank] << left << setw(2) <<  setfill(' ') << suit_mark[player_cards[0][i].suit];
	}
	cout << endl;
}

void print_field(const bool field[][13]){
	for(int i=0;i<4;i++){
		cout << "       " << "Suit:" <<  suit_mark[i] << endl;
		for(int k=0;k<13;k++){
			if(field[i][k] == true) cout << left << setw(3) << rank_mark[k + 1];
			else cout << setw(2) << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

void set_player_cards(Card player_cards[][13],int player_number,queue<Card> &work){
	int i=0;
	while(! work.empty()){
		player_cards[player_number][i] = work.front();
		work.pop();
		i++;
	}
}

void push_card_work(Card player_cards[][13],int player_num,int hands_cnt[],Card not_push,queue<Card> &work){
	//cout << "push_hands:" << hands_cnt[player_num] << endl;
	for(int i=0;i<hands_cnt[player_num];i++){
		if((not_push.rank == player_cards[player_num][i].rank) &&
		   (not_push.suit == player_cards[player_num][i].suit)) {
			continue;
		}else{
			work.push(player_cards[player_num][i]);
		}
	}
	hands_cnt[player_num]--; //カードを排除し終えた時点で、手札の表示を減らす。
	//cout << "work:" << work.size() << endl;
	set_player_cards(player_cards,player_num,work);
}

int set_seven(bool field[][13],Card player_cards[][13],int hands_cnt[]){
	queue<Card> work;
	int i,k,dia7_owner;
	for(i=0;i<4;i++){
		field[i][6] = true;
		for(k=0;k<13;k++){
			if(player_cards[i][k].rank != 7){ //7をplayer_cardsから除外するための前処理。
				work.push(player_cards[i][k]);
				hands_cnt[i]++; //プレイヤーの手札の枚数を加算している。
			}else if(player_cards[i][k].suit == 2){
				dia7_owner = i;
				//cout << "set_seven:" << i << " " << player_cards[i][k].suit << endl;
			}
		}
		set_player_cards(player_cards,i,work);
	}
	return dia7_owner;
}

bool field_check(bool field[][13],Card player_cards[][13],int num,int player_num){
	bool check = false;
	if(player_cards[player_num][num - 1].rank == 0){
		if(field[player_cards[player_num][num - 1].suit][player_cards[0][num - 1].rank] == true){
			check = true;
			cout << "1" << endl;
		}
	}else if(player_cards[player_num][num - 1].rank == 13){
		if(field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank - 2] == true){
			check = true;
			cout << "2" << endl;
		}
	}else if((field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank - 2]  == true) || (field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank ] == true)){
		check = true;
		cout << "3" << endl;
	}
	cout << player_cards[player_num][num - 1].suit << player_cards[player_num][num - 1].rank << endl;
	return check;
}

bool pass_cnt(int pass[],int player_num){
	if(pass[player_num] > 3) return true;
	else return false;
}

void select_player_card(Card player_cards[][13],bool field[][13],int hands_cnt[]){
	int num;
	bool check;
	Card tmp;
	queue<Card> work;
	while(1){
		cout << "場に出すカードは左から何番目？ 番号を入力：";
		cin >> num;
		check = field_check(field,player_cards,num,0);
		if(check == true){
			//cout << "check:" << check << endl;
			//cout << player_cards[0][num - 1].suit  << ' ' << player_cards[0][num - 1].rank << endl;
			field[player_cards[0][num - 1].suit][player_cards[0][num - 1].rank - 1] = true;
			//print_field(field);
			tmp.rank = player_cards[0][num - 1].rank;
			tmp.suit = player_cards[0][num - 1].suit;
			push_card_work(player_cards,0,hands_cnt,tmp,work);
			print_player1_hand(player_cards,hands_cnt);
		}else{
			cout << "このカードは場に出せません。もう一度。" << endl;
		}
	}
}


void select_computer_card( Card player_cards[][13], bool field[][13],int hands_cnt[],const int player_num){
	int num = 1,map[4][13],pos=-1;
	bool check;
	Card tmp;
	Coordinate cards_idx[52];
	queue<Card> work;
	priority_queue<Coordinate> idx; //降順の場合
	for(int i=0;i<4;i++){ //mapの初期化
		for(int k=0;k<13;k++) map[i][k]=0;
	}
	for(int i=0;i<hands_cnt[player_num];i++){ //自身の手札分を加算
		map[player_cards[player_num][i].suit][player_cards[player_num][i].rank]+=10;
	}
	for(int i=0;i<hands_cnt[0];i++){ //プレイヤー1の手札分を加算
		map[player_cards[0][i].suit][player_cards[0][i].rank]++;
	}
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			if(map[i][k] > 9){
				pos++;
				cards_idx[pos].point = map[i][k];
				cards_idx[pos].row   = i;
				cards_idx[pos].col   = k;
				idx.push(cards_idx[pos]);
			}
		}
	}
	for(int i=0;i<pos+1;i++){
		check = field_check(field,player_cards,num,player_num);
	}
	cout << "プレイヤー" << player_num + 1 << "は " << suit_mark[player_cards[player_num][num].suit]
		 << rank_mark[player_cards[player_num][num].rank] << " を出しました。" << endl;
	tmp.rank = player_cards[player_num][num - 1].rank;
	tmp.suit = player_cards[player_num][num - 1].suit;
	push_card_work(player_cards,player_num,hands_cnt,tmp,work);

}

void game(Card player_cards[][13],bool field[][13],int seven,int hands_cnt[]){
	int i,pass[4]={0};
	while(1){
		print_player1_hand(player_cards,hands_cnt);
		select_player_card(player_cards,field,hands_cnt);
		for(i=0;i<4;i++){
			select_computer_card(player_cards,field,hands_cnt,(seven + i) % 4);
		}
		print_field(field);
	}
}

int main(){
	int hands_cnt[4]={0},seven_dia_player;
	bool cards_field[4][13];
	Card player_cards[4][13];
	stack<Card> deck;
	for(int i=0;i<4;i++) for(int k=0;k<13;k++) cards_field[i][k] = false;
	make_deck(deck);
	//shuffle_cards(deck,52);
	dist_card(deck,player_cards);
	seven_dia_player = set_seven(cards_field,player_cards,hands_cnt);
	//cout << "dia:" << seven_dia_player << endl;
	//game(player_cards,cards_field,seven_dia_player,hands_cnt);
	select_computer_card(player_cards,cards_field,hands_cnt,0);
	cout << "--exit--" << endl;
	return 0;
}
