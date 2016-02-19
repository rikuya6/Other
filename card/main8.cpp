#include<iostream>
#include<iomanip>
#include<algorithm>
#include<queue>
#include<ctime>
#include<cstdlib>
using namespace std;

const string suit_mark = "SHDC";
const string rank_mark[14] = {"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
const int bad_score[] = {1,2,3,4};
const int good_score[] = {100,60,30,0};

struct Card{
	int rank;
	int suit;
	bool operator<(const struct Card & r) const{ //sort関数を利用する場合に判定式となる
		if(rank != r.rank) return (rank < r.rank); //ランクを第1キーとして、スートを第2キー、それぞれの昇順にする。
		if(suit != r.suit) return (suit < r.suit);
		return 0;
	}
};

struct Rank{
	int player_number,score;
	bool operator<(const struct Rank & s) const{
		if(score > s.score) return 1; //スコアの降順にする。
		return 0;
	}
};

void make_deck(Card deck[52]){
	for(int i=0;i< 52;i++){
		deck[i].rank = (i % 13) + 1;
		deck[i].suit = (i % 4);
	}
}

void shuffle_cards(Card cards[],int n){
	int a,b;
	Card work;
	srand(time(NULL));
	for(int i=0;i<100000;i++){
		a = rand() % n;
		b = rand() % n;
		work = cards[a];
		cards[a] = cards[b];
		cards[b] = work;
	}
}

void print_field(const bool field[4][13]){
	cout << endl;
	for(int i=0;i<4;i++){
		cout << "\t"  << "　　　　 　　 　"<< suit_mark[i] << endl;
		cout << "\t";
		for(int k=0;k<13;k++){
			if(field[i][k]) cout<< "　" << rank_mark[k + 1];
			else cout << "　";
		}
		cout << endl << endl << endl;
	}
}

void print_player_hand(const Card cards[4][13],const int player,const int hands_cnt[4]){
	cout << "\t\t";
	for(int i=0;i<hands_cnt[player];i++){
		cout << left << setw(5) << setfill(' ') << i + 1;
	}
	cout << endl;
	cout << "あなたの手札：  ";
	//cout << "\t";
	for(int i=0;i<hands_cnt[player];i++){
		cout << suit_mark[cards[player][i].suit] << left << setw(4) << setfill(' ') << rank_mark[cards[player][i].rank];
	}
	cout << endl;
}

void first_card(Card cards[4][13],const Card deck[52]){
	int index = 0;
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			cards[i][k] = deck[index];
			index++;
		}
	}
}

void delete_hands_card(Card cards[4][13],const int player,int hands_cnt[4], const Card del){
	int idx=0,cnt=0;
	queue<Card> tmp;
	for(int i=0;i<hands_cnt[player];i++){
		if(cards[player][i].rank == del.rank && cards[player][i].suit == del.suit) continue;
		tmp.push(cards[player][i]);
		cnt++;
	}
	hands_cnt[player] = cnt;
	while(!tmp.empty()){
		cards[player][idx] = tmp.front();
		tmp.pop();
		idx++;
	}
	//cout << "hands_cnt " << player << " "  << hands_cnt[player] << endl;
}

void setup_game_field(bool field[4][13],Card cards[4][13],int hands_cnt[4],int &start_player){ //7をフィールドに出す。ダイヤの7を持つ人を探す。
	Card del;
	del.rank = 7;
	for(int i=0;i<4;i++){
		field[i][6] = true; //7を出したものとする。
		for(int k=0;k<13;k++){
			if((cards[i][k].rank) == 7 && (cards[i][k].suit) == 2) start_player = i; //ダイヤの7を持っているプレイヤーを探す。
		}
	}
	//cout << "start_player:" << start_player << endl;
	for(int i=0;i<4;i++){ //7を手札から除外する。
		for(int k=0;k<4;k++){
			del.suit = k;
			delete_hands_card(cards,i,hands_cnt,del);
		}
	}
//	for(int i=0;i<4;i++){
//		print_player_hand(cards,i,hands_cnt);
//	}
}

bool field_check(bool field[4][13],Card cards[4][13],int player,int target){
	if(cards[player][target - 1].rank == 1){ //手札が1なら、場は2を見る。target-1で手札の配列のインデックスになる。rankは1~13。
		if(field[cards[player][target - 1].suit][cards[player][target - 1].rank]){
			//cout << "1" << endl;
			return true; //rankの2がtrueならtrue
		}
	}else if(cards[player][target - 1].rank == 13){
		if(field[cards[player][target - 1].suit][cards[player][target - 1].rank - 2]) {
			//cout << "2" << endl;
			return true;
		}
	}else if((field[cards[player][target - 1].suit][cards[player][target - 1].rank - 2]) || (field[cards[player][target - 1].suit][cards[player][target - 1].rank])){
		//cout << "3" << endl;
		return true;
	}
	//cout << "4" << endl;
	return false;
}

void select_player1_card(Card cards[4][13],bool field[4][13],int hands_cnt[4],int pass[4]){
	int target;
	bool check = false;
	Card del;
	while(1){
		cout << "左から何番目のカードを出すか入力(-1でパス)：";
		cin >> target;
		if(target == -1){
			pass[0]++;
			//cout << "プレイヤー1 pass:" << pass[0] << endl;
			return;
		}
		if(target > hands_cnt[0] || target < 1){
			cout << "1～" << hands_cnt[0] << "までの数字を入力してください。" << endl;
			continue;
		}
		check = field_check(field,cards,0,target);
		if(check){
			field[cards[0][target - 1].suit][cards[0][target - 1].rank - 1] = true;
			del.rank = cards[0][target - 1].rank;
			del.suit = cards[0][target - 1].suit;
			delete_hands_card(cards,0,hands_cnt,del);
			return;
		}
		cout << "このカードは出せません。もう一度選択してください。" << endl;
	}
}

void select_computer_card(Card cards[4][13],bool field[4][13],int player, int hands_cnt[4],int pass[4]){
	bool check = false;
	Card del;
	for(int i=1;i<=hands_cnt[player];i++){
		check = field_check(field,cards,player,i);
		if(check){
			//cout << "hands_cnt" << player + 1 << " " << hands_cnt[player] << endl;
			//print_player_hand(cards,player,hands_cnt);
			cout << "プレイヤー" << player + 1 << "は " << suit_mark[cards[player][i - 1].suit]
				 << rank_mark[cards[player][i - 1].rank] << " を出しました。" << endl;
			field[cards[player][i - 1].suit][cards[player][i - 1].rank - 1] = true;
			del.rank = cards[player][i - 1].rank;
			del.suit = cards[player][i - 1].suit;
			delete_hands_card(cards,player,hands_cnt,del);
			//print_player_hand(cards,player,hands_cnt);
			return;
		}
	}
	cout << "プレイヤー" << player + 1 << "はパスをしました。" << endl;
	pass[player]++;
}

void game_procedure(Card cards[4][13],bool field[4][13],const int start_player,int hands_cnt[4],int pass[4],Rank score[4]){
	int order = start_player,pass_cnt = 0,bad_rank = 0,good_rank = 0;
	bool game_over[4],pass_flag[4];
	for(int i=0;i<4;i++){
		game_over[i] = false;
		pass_flag[i] = false;
	}
	cout << "ゲームスタート!" << endl;
	cout << "プレイヤー" << start_player + 1 << "からスタートです。" << endl;
	while(1){
		pass_cnt = 0;
		for(int i=0;i<4;i++){
			if(game_over[i] == false && pass_flag[i] == true){
				game_over[i] = true;
				score[i].score += bad_score[bad_rank];
				bad_rank++;
			}
			if(pass[i] > 3) pass_cnt++;
		}
		if(pass_cnt >= 3){ //このif文でゲームの中心の処理を終えるか続けるかを決めている。
			cout << "3プレイヤーの結果が決まりました。ラウンド終了です。" << endl;
			for(int i=0;i<4;i++){
				if(! game_over[i]){
					score[i].score += good_score[good_rank];
					good_rank++;
				}
			}
			break;
		}
		if(hands_cnt[order] < 1){
			order++;
			order = order % 4;
			continue;
		}
		if(order == 0){
			print_field(field);
			sort(&cards[0][0],&cards[0][0] + hands_cnt[0]);
			print_player_hand(cards,0,hands_cnt);
			select_player1_card(cards,field,hands_cnt,pass);
			if(pass[0] > 3){
				pass_flag[0] = true;
				hands_cnt[0] = 0;
				cout << "プレイヤー1はパスを4回したため、ゲーム終了です。手札は場には出されません。" << endl;
			}
		}else{
			select_computer_card(cards,field,order,hands_cnt,pass);
			if(pass[order] > 3){
				pass_flag[order] = true;
				hands_cnt[order] = 0;
				cout << "プレイヤー" << order + 1 << "はパスを4回したため、ゲーム終了です。手札は場には出されません。" << endl;
			}
		}
		order++;
		order = order % 4;
	}
}

void initialize_game(bool field[4][13],Card deck[52],Card cards[4][13],int hands_cnt[4],int pass[4],int &start_player){
	for(int i=0;i<4;i++){ //フィールドの初期化
		pass[i] = 0;
		hands_cnt[i] = 13;
		for(int k=0;k<13;k++) field[i][k] = false;
	}
	make_deck(deck);
	//print_field(field);
	shuffle_cards(deck,52);
	first_card(cards,deck);
	//for(int i=0;i<4;i++) print_player_hand(cards,i,hands_cnt);
	setup_game_field(field,cards,hands_cnt,start_player);
}

int main(){
	int hands_cnt[4],start_player=1,pass[4]={0},continue_game = 1;
	Rank player_rank[4] = {0,0,1,0,2,0,3,0};
	Card deck[52],cards[4][13];
	bool field[4][13];
	while(1){
		if(continue_game == 1){ //ゲームの継続判定。
			initialize_game(field,deck,cards,hands_cnt,pass,start_player); //ゲーム毎に初期化の必要があるものを初期化する。
			game_procedure(cards,field,start_player,hands_cnt,pass,player_rank); //ゲームのメイン処理。
			cout << "\t\t-----最終結果-----" << endl;
			print_field(field);
			//for(int i=0;i<4;i++) print_player_hand(cards,i,pass);
			cout << "ラウンド終了" << endl << endl;
			while(1){ //ゲームの継続処理。
				cout << "ゲームをまだ続けますか？ （１：継続.０：終了）値を入力：";
				cin >> continue_game;
				if(continue_game == 1 || continue_game == 0) break;
				else{
					cout << "入力された値が間違っています。" << endl;
					continue;
				}
			}
		}else{ //ゲームをやめる場合の判定。最初のif文のelse。
			break;
		}
	}
	sort(player_rank,player_rank + 4);
	cout << "\tゲーム結果" << endl;
	for(int i=0;i<4;i++){

		cout << "\t" << i + 1 << "位：プレイヤー" << player_rank[i].player_number + 1 << "　総得点：" << player_rank[i].score  << endl;
	}
	cout << "ゲーム終了です。" << endl;
	return 0;
}
