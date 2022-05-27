#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <utility>
#include <set>
#include <fstream>

typedef std::unordered_map<std::string, std::size_t> admins_t;
typedef std::unordered_map<std::string, std::size_t> accounts_t;
typedef std::unordered_map<std::string, std::string> secrets_t;

const std::hash<std::string> str_hasher;

std::string get_input() {
  std::string input;
  std::getline(std::cin, input);

  return input;
}

std::string get_input(const std::string& output) {
  std::cout << output;

  return get_input();
}

void make_lower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });
}

std::string get_username() {
  std::string input;

  while (true) {
    input = get_input("username: ");

    if (!input.empty())
      break;

    std::cout << "username cannot be empty, please try again\n";
  }

  return input;
}

std::string get_password() {
  std::string input;

  while (true) {
    input = get_input("password: ");

    if (!input.empty())
      break;

    std::cout << "password cannot be empty, please try again\n";
  }

  return input;
}

void delete_user(accounts_t& accounts, secrets_t& secrets) {
  std::cout << "users: \n";

  for (const auto& kv : accounts)
    std::cout << kv.first << '\n';
  
  while (true) {
    std::string username = get_input("user to delete: ");

    if (accounts.contains(username)) {
      accounts.erase(username);
      secrets.erase(username);

      break;
    }

    std::cout << "invalid username, please try again\n";
  }
}

void unlock_account(std::set<std::string>& locked_accounts) {
  std::cout << "locked users: \n";

  for (const auto& user : locked_accounts)
    std::cout << user << '\n';

  while (true) {
    std::string username = get_input("user to unlock: ");

    if (locked_accounts.contains(username)) {
      locked_accounts.erase(username);

      break;
    }

    std::cout << "invalid username, please try again\n";
  }
}

void admin_process(accounts_t& accounts,
  std::set<std::string>& locked_accounts, secrets_t& secrets)
{
  while (true) {
    std::string input = get_input("[delete user] or [unlock account]?: ");

    make_lower(input);

    if (input == "delete user")
      delete_user(accounts, secrets);

    else if (input == "unlock account")
      unlock_account(locked_accounts);

    else {
      std::cout << "unrecognised input, please try again\n";

      continue;
    }

    break;
  }
}

void login_process(const accounts_t& admins, accounts_t& accounts,
  std::set<std::string>& locked_accounts, secrets_t& secrets)
{
  std::string username;
  std::string password;
  std::unordered_map<std::string, int> attempts;

  while (true) {
    username = get_username();
    password = get_password();

    if (admins.contains(username) &&
      admins.at(username) == str_hasher(password))
    {
      admin_process(accounts, locked_accounts, secrets);
      
      return;
    }

    else if (locked_accounts.contains(username)) {
      std::cout << "account locked, please contact an admin\n";
      
      return;
    }

    else if (accounts.contains(username) &&
      accounts.at(username) == str_hasher(password))
    {
      break;
    }

    if (!attempts.contains(username))
      attempts.insert({username, 1});

    else
      attempts[username] += 1;

    if (attempts.at(username) >= 3) {
      locked_accounts.insert(username);

      std::cout << "too many unsuccessful login attempts, account locked\n";
    }

    else
      std::cout << "invalid account details, please try again\n";
  }

  std::cout << secrets.at(username) << '\n';
}

void register_process(const accounts_t& admins, accounts_t& accounts,
  secrets_t& secrets)
{
  std::string username;

  while (true) {
    username = get_username();

    if (!admins.contains(username) && !accounts.contains(username))
      break;
      
    std::cout << "account with this username already exists, "
      "please try again\n";
  }

  accounts.insert({username, str_hasher(get_password())});
  secrets.insert({username, get_input("secret: ")});
}

// void save_data(const accounts_t& accounts,
//   const std::set<std::string>& locked_accounts)
// {
//   std::ofstream ofs("accounts");

//   for ()
// }

int main() {
  const admins_t admins = {
    {"admin", str_hasher("password")}
  };

  accounts_t accounts;
  std::set<std::string> locked_accounts{};
  secrets_t secrets;

  while (true) {
    std::string input = get_input("[login] or [register]?: ");

    make_lower(input);

    if (input == "login")
      login_process(admins, accounts, locked_accounts, secrets);

    else if (input == "register")
      register_process(admins, accounts, secrets);

    else {
      std::cout << "unrecognised input, please try again\n";

      continue;
    }

    // save data
  }

  return 0;
}