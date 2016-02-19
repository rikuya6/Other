#include<iostream>
#include<iomanip>
#include<algorithm>
#include<queue>
#include<ctime>
#include<climits>
#include<cstdlib>
using namespace std;

const string suit_mark = "SHDC";
const string rank_mark[14] = {"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
const int score_point[] = {100,60,30,0}; //1位から順に100点,60,30,0

struct Card{ //トランプのスートとランクの構造体
	int rank;
	int suit;
	bool operator<(const struct Card & r) const{ //sort関数を利用する場合に判定式となる
		if(suit != r.suit) return (suit < r.suit); //スートを第1キー、ランクを第2キーとして、それぞれの昇順にする。
		if(rank != r.rank) return (rank < r.rank);
		return 0;
	}
};

struct Rank{ //順位計算とコンピュータプレイヤーのトランプ選択に利用
	int player_number,score;
	bool operator<(const struct Rank & s) const{
		if(score > s.score) return 1; //スコアの降順にする。
		return 0;
	}
};

void make_deck(Card deck[52]){ //52枚のトランプデッキを作る。
	for(int i=0;i< 52;i++){
		deck[i].rank = (i % 13) + 1;
		deck[i].suit = (i % 4);
	}
}

void shuffle_cards(Card cards[],int n){ //デッキをシャッフルする。
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

void first_card(Card cards[4][13],const Card deck[52]){ //ゲームの最初に、プレイヤー全員に13枚ずつトランプを配る関数。
	int index = 0;
	for(int i=0;i<4;i++){
		for(int k=0;k<13;k++){
			cards[i][k] = deck[index];
			index++;
		}
	}
}

void delete_hands_card(Card cards[4][13],const int player,int hands_cnt[4], const Card del){ //選択したトランプを選択したプレイヤーの手札から消す。
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

void print_field(const bool field[4][13]){ //場に出ているトランプを表示する。
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

void print_player_hand(const Card cards[4][13],const int player,const int hands_cnt[4]){ //プレイヤーの手札を表示する。
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

bool field_check(bool field[4][13],Card cards[4][13],int player,int target){ //場に出せるかの判定をする。
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

int  input_player1(Card cards[4][13], bool field[4][13], int hands_cnt[4], int pass[4]){ //人間プレイヤーが場に出すトランプが何かを入力させる処理。
	int target,check_cnt = 0;
	for(int i=1;i<hands_cnt[0] + 1;i++){ //場に出せるトランプの枚数を数えている。
		if(field_check(field,cards,0,i)) check_cnt++;
	}
	while(1){
		if(check_cnt > 0) cout << "左から何番目のカードを出すか入力(-1でパス)" << "パスは" << pass[0] << "回しています。\n"; //出せるトランプがある場合
		else {
			cout << "出せるトランプが無いようです。\n";
			cout << "パスを選択してください。(-1でパス) パスは" << pass[0] << "回しています。\n";
		}
		cout << "入力待ち中：";
		cin >> target;
		if(!cin.good()){
			cout << "入力が間違っているようです。再入力してください。" << endl;
			cin.clear();
			cin.ignore(INT_MAX,'\n');
			continue;
		}
		cout << endl;
		if(target == -1) break;
		if(target > hands_cnt[0] || target < 1){
			cout << "1～" << hands_cnt[0] << "までの数字を入力してください。" << endl;
			continue;
		}else{
			break;
		}

	}
	return target;
}

void select_player1_card(Card cards[4][13],bool field[4][13],int hands_cnt[4],int pass[4]){ //人間プレイヤーが場に出すトランプを選ぶ。
	int target;
	bool check = false;
	Card del;
	while(1){
		target = input_player1(cards,field,hands_cnt,pass);
		if(target == -1){
			pass[0]++;
			//cout << "プレイヤー1 pass:" << pass[0] << endl;
			return;
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

void select_computer_card(Card cards[4][13],bool field[4][13],int player, int hands_cnt[4],int pass[4]){ //コンピュータプレイヤーがトランプを選ぶ
	bool check = false;
	int field_point[4][13],check_cnt = 0,pos = 0;
	Card del;
	Rank index[13];
	for(int i=0;i<4;i++){ //フィールドポイントの初期化
		for(int k=0;k<13;k++){
			field_point[i][k] = 0;
		}
	}
	for(int i=0;i<hands_cnt[0];i++){ //どこに出すと邪魔できるか。できないならば-5
		if(cards[0][i].rank == 1){
			field_point[cards[0][i].suit][cards[0][i].rank] -= 5;
		}else if(cards[0][i].rank == 13){
			field_point[cards[0][i].suit][cards[0][i].rank - 2] -= 5;
		}else{
			field_point[cards[0][i].suit][cards[0][i].rank - 2] -= 5;
			field_point[cards[0][i].suit][cards[0][i].rank] -= 5;
		}
	}
	for(int i=1;i<=hands_cnt[player];i++){
		check = field_check(field,cards,player,i);
		if(check){
			check_cnt++;
			index[pos].score = field_point[cards[player][i - 1].suit][cards[player][i - 1].rank - 1];
			index[pos].player_number = i;
			pos++;
		}
	}
//	for(int i=0;i<4;i++){ //フィールドポイントを表示する
//		for(int k=0;k<13;k++){
//			cout  << field_point[i][k];
//		}
//		cout << endl;
//	}
	sort(index,index + pos);
	if(check_cnt > 0){
		int i = index[0].player_number;
//		cout << "hands_cnt" << player + 1 << " " << hands_cnt[player] << endl;
//		print_player_hand(cards,player,hands_cnt);
//		for(int i=0;i<check_cnt;i++){ //お邪魔ポイントが何ポイントか表示する。
//			cout << index[i].player_number << " " << index[i].score << endl;
//		}
		cout << "プレイヤー" << player + 1 << "は " << suit_mark[cards[player][i - 1].suit]
														   << rank_mark[cards[player][i - 1].rank] << " を出しました。" << endl;
		field[cards[player][i - 1].suit][cards[player][i - 1].rank - 1] = true;
		del.rank = cards[player][i - 1].rank;
		del.suit = cards[player][i - 1].suit;
		delete_hands_card(cards,player,hands_cnt,del);
		//print_player_hand(cards,player,hands_cnt);
		return;
	}
	cout << "プレイヤー" << player + 1 << "はパスをしました。" << endl;
	pass[player]++;
}

void game_procedure(Card cards[4][13],bool field[4][13],const int start_player,int hands_cnt[4],int pass[4],Rank score[4]){ //ゲームの流れに従って処理していく
	int order = start_player,pass_cnt = 0,rank = 0;
	bool pass_flag[4];
	for(int i=0;i<4;i++) pass_flag[i] = false;
	cout << "ゲームスタート!" << endl;
	cout << "プレイヤー" << start_player + 1 << "からスタートです。" << endl;
	while(1){
		if(order == 0 && pass_flag[0] == false && hands_cnt[0] > 0){
			print_field(field);
			sort(&cards[0][0],&cards[0][0] + hands_cnt[0]);
			print_player_hand(cards,0,hands_cnt);
			select_player1_card(cards,field,hands_cnt,pass);
			if(pass[0] > 3 && pass_flag[order] == false){
				pass_flag[0] = true;
				pass_cnt++;
				hands_cnt[0] = 0;
				cout << "プレイヤー1はパスを4回したため、ゲーム終了です。手札は場には出されません。" << endl;
			}
		}else if(order != 0 && pass_flag[order] == false && hands_cnt[order] > 0){
			select_computer_card(cards,field,order,hands_cnt,pass);
			//cout << "hands_cnt:" << order + 1 << " " <<  hands_cnt[order] << endl;
			if(pass[order] > 3 && pass_flag[order] == false){
				pass_flag[order] = true;
				pass_cnt++;
				hands_cnt[order] = 0;
				cout << "プレイヤー" << order + 1 << "はパスを4回したため、ゲーム終了です。手札は場には出されません。" << endl;
			}
		}
		if(pass_cnt >= 3){
			cout << "手札をなくしたプレイヤー、または、4回パスをしたプレイヤーが3プレイヤー以上でました。ゲーム終了です。" << endl;
			for(int i=0;i<4;i++){
				if(!pass_flag[i]) score[i].score += score_point[rank];
			}
			return;
		}
		if(hands_cnt[order] < 1 && pass_flag[order] == false){
			pass_flag[order] = true;
			pass_cnt++;
			score[order].score += score_point[rank];
			rank++;
		}
		order++;
		order = order % 4;
	}
}

void initialize_game(bool field[4][13],Card deck[52],Card cards[4][13],int hands_cnt[4],int pass[4],int &start_player){ //初期化を行い2戦目以上をスタートできるようにする
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

int input_continue(){ //もう一度ゲームを行うのか確認する入力をさせる処理。
	int flag;
	while(1){ //ゲームの継続処理。
		cout << "ゲームをもう一度行いますか？ （１：継続 ０：終了）値を入力：" << endl;
		cout << "入力待ち中:" ;
		cin >> flag;
		if(!cin.good()){ //不正な入力の場合。入力が無かったことにする。
			cout << "入力がおかしいようです。もう一度入力してください。" << endl;
			cin.clear();
			cin.ignore(INT_MAX,'\n');
			continue;
		}
		cout << endl;
		if(flag == 1 || flag == 0) break; //正しい値か？
		else{
			cout << "1 または 0を入力してください。" << endl;
			continue;
		}
	}
	return flag;
}

int main(){ //ゲームの繰り返し処理と順位表示をする。
	int hands_cnt[4],start_player=1,pass[4]={0},continue_game = 1,rank = 1,old_point = 0;
	Rank player_rank[4];
	for(int i=0;i<4;i++){ //初期化はプログラム実行中一度しか行わない。
		player_rank[i].player_number = i;
		player_rank[i].score = 0;
	}
	Card deck[52],cards[4][13];
	bool field[4][13];
	while(1){
		if(continue_game == 0) break; //ゲームの継続判定 0場合は終了。1の時は継続。
		else{
			initialize_game(field,deck,cards,hands_cnt,pass,start_player); //ゲーム毎に初期化の必要があるものを初期化する。
			game_procedure(cards,field,start_player,hands_cnt,pass,player_rank); //ゲームのメイン処理。
			cout << "\t\t-----最終結果-----\n";
			print_field(field);
			//for(int i=0;i<4;i++) print_player_hand(cards,i,pass);
			cout << "ラウンド終了\n";
			cout << "あなたは、" << player_rank[0].score - old_point << "点を獲得しました。\n\n";
			old_point += player_rank[0].score - old_point;
			//cout << player_rank[0].score << endl;
			continue_game = input_continue(); //ゲームを継続するかの入力をさせる。
		}
	}
	cout << "\t     -----ゲーム結果-----\n";//総合順位表示
	sort(player_rank,player_rank + 4);
	cout << "\t" << rank << "位：プレイヤー" << player_rank[0].player_number + 1 << "　総得点：" << player_rank[0].score  << endl;
	for(int i=1;i<4;i++){
		if(player_rank[i].score !=  player_rank[i - 1].score) rank++;
		cout << "\t" << rank << "位：プレイヤー" << player_rank[i].player_number + 1 << "　総得点：" << player_rank[i].score  << endl;
	}
	cout << "ゲーム終了です。\n";
	return 0;
}
