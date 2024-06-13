#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

// Transaction 类
class Transaction {
    public:
        int id;
        string date;
        string reason;
        string amount;
        string path;
        string counterparty;
        string note;
    
    // 构造函数
    Transaction(int id = 0,
                string date = "",
                string reason = "",
                string amount = "0",
                string path = "",
                string counterparty = "",
                string note = "")
        : id(id), date(date), reason(reason), amount(amount), path(path), counterparty(counterparty), note(note) {}
    
    // 转换为json对象
    json Turn_to_JSON() const {
        return {
            {"id", id},
            {"date", date},
            {"reason", reason},
            {"amount", amount},
            {"path", path},
            {"counterparty", counterparty},
            {"note", note}
        };
    }

    // 重载运算符 <<
    friend ostream& operator<<(ostream& os, const Transaction& t){
        os << "Transaction " << t.id << ": " << endl
           << "date: " << t.date << endl
           << "amount: " << t.amount << endl
           << "reason: " << t.reason << endl
           << "path: " << t.path << endl
           << "counterparty: " << t.counterparty << endl
           << "note: " << t.note << endl;
        return os;
    }
};

// 加载文件
vector<Transaction> load_bills(const string& filename) {
    // 构建Transaction示例的vector容器
    vector<Transaction> transactions;

    // 打开文件
    ifstream i(filename);

    // 检查文件是否正常打开，若未能成功打开说明文件不存在，构造文件
    if (!i.is_open()) {
        ofstream o(filename);
        json j = {{}};
        o << j;
        return transactions;
    }

    // 读取json数据
    json j;
    i >> j;

    // 检查文件是否是json数组
    if (!j.is_array()) {
        cerr << "Error: File does not contain a JSON array." << endl;
        i.close();
        return transactions;
    }

    // 创建Transaction对象
    for (auto& elem : j) {
        // 提取数据
        int id = elem["id"];
        string date = elem["date"];
        string reason = elem["reason"];
        string amount = elem["amount"];
        string path = elem["path"];
        string counterparty = elem["counterparty"];
        string note = elem["note"];

        // 创建对象并填入数据
        transactions.emplace_back(id, date, reason, amount, path, counterparty, note);
    }

    // 关闭文件
    i.close();
    return transactions;
}

// 保存文件
void save_bills(const vector<Transaction>& transactions, const string& filename) {
    // 创建一个JSON数组保存交易
    json j = json::array();
    for (const auto& transaction : transactions) {
        j.push_back(transaction.Turn_to_JSON());
    }

    // 打开文件
    ofstream o(filename);

    // 检查文件是否正常打开
    if(!o.is_open()) {
        cerr << "Error: File-opening Failure: " << filename << endl;
        return;
    }

    // 将JSON数组写入文件
    o << setw(4) << j << endl;

    // 关闭文件
    o.close();
}

// 计算id
int get_next_transaction_id(vector<Transaction>& transactions) {
    // 预设最大id的底值，以防文件为空列表
    int max_id = 0;

    // 获取最大id
    for (const auto& transaction : transactions) {
        if (transaction.id > max_id) {
            max_id = transaction.id;
        }
    }

    // 返回新最大id
    return max_id + 1;
}

// 添加交易
void add_transaction(vector<Transaction>& transactions,
                    string date, string reason, string amount, string path, string counterparty, string note)
{
    // 生成id
    int id = get_next_transaction_id(transactions);

    // 创建新Transaction对象
    Transaction new_transaction = Transaction(id, date, reason, amount, path, counterparty, note);

    // 新对象加入容器
    transactions.push_back(new_transaction);
}

// 删除交易
void delete_transaction_id(vector<Transaction>& transactions, int target_id) {
    // 第一步：删除指定ID的交易
    auto it = remove_if(transactions.begin(), transactions.end(), 
                    [target_id](const Transaction& t) {return t.id == target_id;});
    // 如果没有找到指定交易
    if (it == transactions.end()) {
        // 输出错误信息
        cerr << "Error: Transaction Not Found." << endl;
    }
    // 如果找到指定交易
    else {
        // 移除交易
        transactions.erase(it, transactions.end());
        // 第二步：重新计算剩余id
        int newID = 1;
        for (Transaction& t : transactions) {
            t.id = newID++;
        }
        cout << "Delete transaction success!" << endl;
    }
}

// 关键词判断函数
bool containKeyword(const Transaction& transaction, const string& keyword) {
    return transaction.date.find(keyword) != string::npos ||
           transaction.reason.find(keyword) != string::npos ||
           transaction.path.find(keyword) != string::npos ||
           transaction.counterparty.find(keyword) != string::npos ||
           transaction.note.find(keyword) != string::npos;
}

// 查找交易
vector<Transaction> find_transaction(const vector<Transaction>& transactions, const string& keyword) {
    vector<Transaction> found_transactions;
    for (const auto& transaction : transactions) {
        if (containKeyword(transaction, keyword)) {
            found_transactions.push_back(transaction);
        }
    }
    // 如果没有找到交易
    if (found_transactions.empty()) {
        cerr << "Error: Transaction Not Found.";
        return found_transactions;  // 解决警告的，没有实在意义
    }
    // 如果找到交易
    else {
        return found_transactions;
    }
}

// 修改交易
void modify_transaction(vector<Transaction>& transactions, int id, string field, string newdata) {
    for (auto& transaction : transactions) {
        if (transaction.id == id) {
            if (field == "date") {
                transaction.date = newdata;
                cout << "Modify transaction success!";
            }
            else if (field == "reason") {
                transaction.reason = newdata;
                cout << "Modify transaction success!";
            }
            else if (field == "amount") {
                transaction.amount = newdata;
                cout << "Modify transaction success!";
            }
            else if (field == "path") {
                transaction.path = newdata;
                cout << "Modify transaction success!";
            }
            else if (field == "counterparty") {
                transaction.counterparty = newdata;
                cout << "Modify transaction success!";
            }
            else if (field == "note") {
                transaction.note = newdata;
                cout << "Modify transaction success!";
            }
            else {
                cerr << "Error: Transaction Not Found." << endl;
            }
        }
    }
}

// 打包函数

int main(){
    cout << "This is FINANCIAL LOG TOOL CLI 2024.6, authored by Bgc and Chen X." << endl
         << "Input the word [help] to get our guidelines." << endl;
    string file_name = "sheet.json";
    while(true){
        // 先换行，提高视觉观感
        cout << endl;

        // 命令提示
        cout << "Fltcli> ";

        // 获取命令
        string command;
        cin >> command;

        // 命令判断
        // 如果是 help
        if (command == "help"){
            cout << "[Usage] Fltcli> command" << endl
                 << "command ranges from [add, delete, find, modify]" << endl;
        }
        // 如果是 quit / exit
        else if (command == "quit" or command == "exit") {
            // 再见
            return 0;
        }
        // 如果是 add
        else if (command == "add") {
            // 新建容器，承接加载函数的数据
            vector<Transaction> transactions;
            transactions = load_bills(file_name);

            // 提示用法
            cout << "Input the date, the reason, the amount, the path, the counterparty, the note of transaction in turns." << endl
                 << "Fltcli add> ";

            // 获取数据
            string date, reason, path, counterparty, note, amount;
            cin >> date >> reason >> amount >> path >> counterparty >> note;

            // 添加交易
            add_transaction(transactions, date, reason, amount, path, counterparty, note);

            // 保存文件
            save_bills(transactions, file_name);

            // 命令成功提示
            cout << "Add transaction success!" << endl;
        }
        // 如果是 delete
        else if (command == "delete") {
            // 新建容器，承接加载函数的数据
            vector<Transaction> transactions;
            transactions = load_bills(file_name);

            // 提示用法
            cout << "Input the id of the target transaction." << endl
                 << "Fltcli delete> ";
            
            // 获取数据
            int id;
            cin >> id;

            // 删除交易
            delete_transaction_id(transactions, id);

            // 保存文件
            save_bills(transactions, file_name);
        }
        // 如果是 find
        else if (command == "find") {
            // 新建容器，承接加载函数的数据
            vector<Transaction> transactions;
            vector<Transaction> found_transactions;
            transactions = load_bills(file_name);

            // 提示用法
            cout << "Input the keyword of the target transactions." << endl
                 << "Fltcli find> ";

            // 获取数据
            string keyword;
            cin >> keyword;

            // 查找交易
            found_transactions = find_transaction(transactions, keyword);
            for(const auto& transaction : found_transactions) {
                // 输出交易
                cout << transaction << endl;
            }
        }
        // 如果是 modify
        else if (command == "modify") {
            // 新建容器，承接加载函数的数据
            vector<Transaction> transactions;
            transactions = load_bills(file_name);

            // 提示用法
            cout << "Input the id, the item, the newdata of the target transaction in order." << endl
                 << "Fltcli modify> ";
            
            // 获取数据
            int id;
            string field, newdata;
            cin >> id >> field >> newdata;

            // 修改交易
            modify_transaction(transactions, id, field, newdata);

            // 保存文件
            save_bills(transactions, file_name);
        }
        // 如果啥也不是（？）
        else {
            cerr << "Error: Syntax Error: Check your inputs" << endl;
        }
    }
}