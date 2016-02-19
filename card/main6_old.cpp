#include<iostream>
#include<iomanip>
#include<algorithm>
#include<queue>
#include<stack>
#include<ctime>
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

void make_deck(stack<Card> &deck){
	Card work;
	for(int i=0;i<52;i++){
		work.rank = (i % 13)+1;
		work.suit = i % 4;
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

void print_player_hand(Card player_cards[4][13],int player_num,int hands_cnt[4]){
	cout << hands_cnt[player_num] << endl;
	cout << "プレイヤー" << player_num << "のカード" << endl;
	for(int i=0;i<hands_cnt[player_num];i++){
			cout << left << setw(3) << setfill(' ') << i + 1;
	}
	cout << endl;
	for(int i=0;i<hands_cnt[player_num];i++){
		cout << rank_mark[player_cards[player_num][i].rank] << left << setw(2) <<  setfill(' ') << suit_mark[player_cards[player_num][i].suit] ;
	}
	cout << endl;
}

void print_field(bool field[4][13]){
	for(int i=0;i<4;i++){
		cout << "       " << "Suit:" <<  suit_mark[i] << endl;
		for(int k=0;k<13;k++){
			if(field[i][k]) cout << left << setw(3) << rank_mark[k + 1];
			else cout << setw(2) << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

void set_player_cards(Card player_cards[4][13],int player_number,queue<Card> &work){
	int i=0;
	while(! work.empty()){
		//cout << "w:" << work.size() << " " << work.front().rank << endl;;
		player_cards[player_number][i] = work.front();
		work.pop();
		i++;
	}
}

void push_card_work(Card player_cards[4][13],int player_num,int hands_cnt[4],Card not_push,queue<Card> &work){
//	cout << "push_hands:" << hands_cnt[player_num] << endl;
	for(int i=0;i < hands_cnt[player_num];i++){
		if(not_push.rank != player_cards[player_num][i].rank && not_push.suit != player_cards[player_num][i].suit){
			work.push(player_cards[player_num][i]);
		}
	}
//	cout << "work:" << work.size() << endl;
//	cout << "hands" << hands_cnt[player_num] << endl;
	hands_cnt[player_num]--; //カードを排除し終えた時点で、手札の表示を減らす。
	set_player_cards(player_cards,player_num,work);
}

int set_seven(bool field[4][13],Card player_cards[4][13],int hands_cnt[4]){
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

bool field_check(bool field[4][13],Card player_cards[4][13],int num,int player_num){
	if(player_cards[player_num][num - 1].rank == 1){
		if(field[player_cards[player_num][num - 1].suit][player_cards[0][num - 1].rank]){
			cout << "field_check: 1" << endl;
			return true;
		}
	}else if(player_cards[player_num][num - 1].rank == 13){
		if(field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank - 2]){
			cout << "field_check: 2" << endl;
			return true;
		}
	}else if(player_cards[player_num][num - 1].rank > 1 && player_cards[player_num][num - 1].rank < 13){
		if(field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank - 2]){
			cout << "field_check: 3" << endl;
			return true;
		}
	}else if(player_cards[player_num][num - 1].rank > 1 && player_cards[player_num][num - 1].rank < 13){
		if(field[player_cards[player_num][num - 1].suit][player_cards[player_num][num - 1].rank]){
			cout << "field_check: 4" << endl;
			return true;
		}
	}
	cout << "field_check: 5" << endl;
	return false;
}

void select_player_card(Card player_cards[4][13],bool field[4][13],int hands_cnt[4],int pass[4]){
	int num;
	bool check;
	Card tmp;
	queue<Card> player_cards_work;
	while(1){
		cout << "場に出すカードは左から何番目？ パスは0を入力。番号を入力：";
		cin >> num;
		if(num == 0){
			pass[0]++;
			return;
		}
		check = field_check(field,player_cards,num,0);
		if(check){
			//cout << "check:" << check << endl;
			//cout << player_cards[0][num - 1].suit  << ' ' << player_cards[0][num - 1].rank << endl;
			field[player_cards[0][num - 1].suit][player_cards[0][num - 1].rank - 1] = true;
			//print_field(field);
			tmp.rank = player_cards[0][num - 1].rank;
			tmp.suit = player_cards[0][num - 1].suit;
			push_card_work(player_cards,0,hands_cnt,tmp,player_cards_work);
			//print_player1_hand(player_cards,hands_cnt);
			return;
		}else{
			cout << "このカードは場に出せません。もう一度。" << endl;
		}
	}
}

void select_computer_card(Card player_cards[4][13],bool field[4][13],int hands_cnt[4],int player_num,int pass[4]){
	int number; //場に出すカードの手札の番号（左から何番目のカードか）
	bool check = false;
	Card tmp;
	queue<Card> work;
	//print_player_hand(player_cards,player_num,hands_cnt);
	for(int i=0; i < hands_cnt[player_num];i++){
		check = field_check(field,player_cards,i,player_num);
		if(check){
			number = i;
			field[player_cards[player_num][number - 1].suit][player_cards[player_num][number - 1].rank - 1] = true;
			cout << "プレイヤー" << player_num + 1 << "は " << suit_mark[player_cards[player_num][number - 1].suit]
				 << rank_mark[player_cards[player_num][number - 1].rank] << " を出しました。" << endl;
			tmp.rank = player_cards[player_num][number - 1].rank;
			tmp.suit = player_cards[player_num][number - 1].suit;
			push_card_work(player_cards,player_num,hands_cnt,tmp,work);
			return;
		}
	}
	pass[player_num]++;
	cout << "プレイヤー" << player_num + 1 << "はパスをしました。" << endl;
}

void game(Card player_cards[4][13],bool field[4][13],int seven,int hands_cnt[4]){
	int i=seven,nohands_cnt=0,pass[4]={0};
	cout << "ゲームスタート！！" << endl;
	cout << "プレイヤー" << seven + 1 << "からスタートです。" << endl;
	while(1){
//		for(int i=0;i<4;i++) {
//			for(int k=0;k<hands_cnt[i];k++){
//				cout << left << setw(3) << setfill(' ') << player_cards[i][k].rank;
//			}
//			cout << endl;
//		}
		nohands_cnt=0;
//		for(int i=0;i<4;i++) cout << hands_cnt[i] << " ";
//		cout << endl;
		for(int k=0;k<4;k++) if(hands_cnt[k] < 1) nohands_cnt++;
		if(nohands_cnt >= 3){
			cout << "3人のプレイヤーが手札がなくなった。ゲーム終了" << endl;
			return;
		}
		if(i != 0 && hands_cnt[i] > 0){
			select_computer_card(player_cards,field,hands_cnt, i,pass);
			//print_field(field);
		}else if(i == 0 && pass[0] < 4){
			print_field(field);
			print_player_hand(player_cards,0,hands_cnt);
			select_player_card(player_cards,field,hands_cnt,pass);
			//print_field(field);
		}
		i++;
		i = i % 4;
		for(int k=0;k<4;k++){
			if(pass[k] > 3){
				hands_cnt[k] = 0;
			}
		}
	}
	for(int i=0;i<4;i++) cout << hands_cnt[i] << endl;
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
	game(player_cards,cards_field,seven_dia_player,hands_cnt);
	cout << "--exit--" << endl;
	return 0;
}
