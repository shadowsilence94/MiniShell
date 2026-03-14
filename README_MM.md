# Minishell Project - Burmese Explanation (မင်းရဲ့ Shell အသစ်စက်စက်)

ဤ file တွင် Minishell project ၏ အလုပ်လုပ်ပုံအဆင့်ဆင့်၊ အရေးကြီးသော function များ၊ variable များနှင့် structure များကို မြန်မာလို အသေးစိတ် ရှင်းပြထားပါသည်။

---

## 🏗️ Project Architecture (ဖွဲ့စည်းပုံ)

Project ကို အပိုင်း (Modules) ကြီး ၄ ခု ခွဲခြားထားသည်။

1.  **Parsing (ခွဲခြမ်းစိတ်ဖြာခြင်း)**: User ရိုက်ထည့်လိုက်သော Command များကို ကွန်ပျူတာနားလည်သော ပုံစံပြောင်းခြင်း။
2.  **Execution (အလုပ်လုပ်ဆောင်ခြင်း)**: ပြောင်းလဲပြီးသား Command များကို လက်တွေ့ Run ခြင်း။
3.  **Builtins (အသင့်ပါသော command များ)**: Shell တွင် မူလပါဝင်သော Command များ (`cd`, `echo`, `exit` etc.)
4.  **Signals (Signal ဖမ်းခြင်း)**: `Ctrl-C` နှင့် `Ctrl-\` ကို လက်ခံစီမံခြင်း။

---

## 🛠️ Data Structures (အသုံးပြုထားသော struct များ)

### `t_token` (Token ဆိုတာဘာလဲ?)
Command line တစ်ခုလုံးကို အပိုင်းငယ်လေးတွေ (Tokens) ဖြတ်လိုက်ခြင်းဖြစ်သည်။
ဥပမာ: `ls -l | grep a` ကို `ls`, `-l`, `|`, `grep`, `a` ဟုခွဲလိုက်သည်။

```c
typedef struct s_token
{
    char            *value; // Token စာသား (ဥပမာ "ls")
    t_token_type    type;   // အမျိုးအစား (WORD, PIPE, REDIRECT, etc.)
    struct s_token  *next;  // နောက် Token ကို ညွှန်ပြသည်။
}   t_token;
```

### `t_command` (Command တစ်ခုလုံး)
Token များကိုစုစည်းပြီး တကယ် run မည့် command ပုံစံပြောင်းထားခြင်းဖြစ်သည်။ Pipe (`|`) ဖြင့် ချိတ်ဆက်ထားသော command များအတွက် linked list ဖြစ်သည်။

```c
typedef struct s_command
{
    char                **args;    // Command နှင့် arguments များ (ဥပမာ ["ls", "-l", NULL])
    char                *cmd_path; // Command ရှိရာလမ်းကြောင်း (ဥပမာ "/bin/ls")
    t_infile            *infiles;  // Input redirection (<)
    t_outfile           *outfiles; // Output redirection (>)
    int                 fd_in;     // Input file descriptor
    int                 fd_out;    // Output file descriptor
    struct s_command    *next;     // Pipe ဖြင့်ချိတ်ထားသော နောက် command
}   t_command;
```

---

## 🔄 အလုပ်လုပ်ပုံ အဆင့်ဆင့် (Workflow)

### 1. Main Loop (`src/main.c`)
Shell တစ်ခုသည် အမြဲတမ်း loop ပတ်နေရမည်။ User input ကိုစောင့်၊ ဖတ်၊ အလုပ်လုပ်၊ ပြီးရင် ပြန်စောင့်။
- `readline()`: User ထံမှ input ကိုဖတ်သည်။
- `parse_input()`: Input ကို parse လုပ်သည်။
- `execute_commands()`: Command များကို run သည်။

### 2. Lexer (`src/parsing/lexer.c`)
- **Lexing**: စာကြောင်းကို `SPACE`, `|`, `<`, `>` တို့ကိုသုံးပြီး ခွဲထုတ်ခြင်း။
- **Quote Handling (အရေးကြီးဆုံးအပိုင်း)**:
  - Single Quote (`'...'`): ၎င်းအတွင်းရှိ မည်သည့်စာသားကိုမဆို (ဥပမာ `$VAR`, `\n`) အစစ်အတိုင်း (literal text) ယူသည်။ Parse မလုပ်ပါ။
  - Double Quote (`"..."`): ၎င်းအတွင်းရှိစာသားများကို တစ်စုတစ်စည်းတည်းထားရှိသော်လည်း `$` (environment variables) ကိုတော့ expand လုပ်ပေးသည်။ (တနည်းအားဖြင့် တန်ဖိုးအစားထိုးပေးသည်)
- ရလဒ်: Command line အရှည်ကြီးမှ အဓိပ္ပါယ်ရှိသော `t_token` linked list တစ်ခုရရှိမည်။

### 3. Parser (`src/parsing/parser.c`)
- Lexer မှရသော Token များကို `t_command` linked list သို့ပြောင်းသည်။
- Pipe (`|`) တွေ့လျှင် command အသစ် `curr->next` ကို ဖန်တီးသည်။
- Redirection (`<`, `>`) တွေ့လျှင် `infiles` သို့ `outfiles` ထဲသို့ filename ထည့်သိမ်းသည်။
- ကျန်သော စာသားများကို `args` array ထဲသို့ထည့်သည်။

### 4. Executor (`src/execution/executor.c`)
Command များကို တကယ် run သည့်နေရာဖြစ်သည်။
- **Forking**: `fork()` ကိုသုံးပြီး Process အသစ်တစ်ခုမွေးထုတ်သည်။ Parent process (minishell) သည် child process အလုပ်လုပ်ပြီးဆုံးသည်အထိ `wait` ဖြင့်စောင့်ပေးရသည်။
- **Piping**: `pipe(fd)` ဖြင့် file descriptor ၂ ခု (ဖတ်ရန် နှင့် ရေးရန်) ဖန်တီးသည်။ `dup2` ကိုသုံး၍ ပထမ command ၏ output (`fd[1]`) ကို ဒုတိယ command ၏ input (`fd[0]`) အဖြစ်ချိတ်ဆက်ပေးသည်။
- **Redirection**: `dup2()` ကိုသုံးပြီး Input/Output လမ်းကြောင်းပြောင်းသည်။
    - `< file`: Input ကို file မှဖတ်ရန် STDIN ကို file ၏ fd ဖြင့် အစားထိုးမည်။
    - `> file`: Output ကို file ထဲရေးရန် STDOUT ကို file ၏ fd ဖြင့် အစားထိုးမည် (Truncate)။
    - `>> file`: STDOUT ကို file ၏ fd ဖြင့် ပြောင်းမည် (Append mode)။
- **Execve**: `execve()` function သည် command ကို လက်တွေ့ run ပေးသည်။ Process တစ်ခုလုံးကို အစားထိုးလိုက်သည်။ Command လမ်းကြောင်း (PATH variable မှရှာဖွေထားသော ဥပမာ - `/bin/ls`)၊ args များနှင့် environment variables များ လိုအပ်သည်။

#### Builtins (`src/builtins/`)
- **Parent Process တွင် run ရမည့် Builtins**: `cd`, `exit`, `export`, `unset`။ အဘယ်ကြောင့်ဆိုသော် ၄င်းတို့သည် minishell ၏ ပင်မ environment ကို ပြောင်းလဲစေသောကြောင့် ဖြစ်သည်။ (ဥပမာ: `cd` အလုပ်လုပ်လျှင် parent ၏ PWD ပြောင်းသွားရမည်။ Fork ဖြင့် run လျှင် child ၏ PWD သာပြောင်းပြီး ပျောက်သွားမည်ဖြစ်သည်။)
- **Child Process တွင် run နိုင်သော Builtins**: `echo`, `pwd`, `env`။ ၄င်းတို့သည် Output သာထုတ်ပေးပြီး Environment အပြောင်းအလဲမလုပ်သောကြောင့် Pipe များဖြင့် တွဲသုံးသောအခါ Child Process အနေဖြင့် လွယ်ကူစွာ run နိုင်သည်။

---

## 🚦 Signals (`src/signals.c`)
- **Ctrl-C (`SIGINT`)**: လက်ရှိ command ကို ရပ်လိုက်ပြီး prompt အသစ်တစ်ခုပြန်ပေးရမည်။ `rl_replace_line("", 0)` နှင့် `rl_redisplay()` ကိုသုံးထားသည်။
- **Ctrl-\ (`SIGQUIT`)**: ပုံမှန်အားဖြင့် ဘာမှမလုပ်ပါ။ (`SIG_IGN` - Ignore)
- **Ctrl-D (EOF)**: `readline` က `NULL` ပြန်ပေးသောအခါ Shell ကို ပိတ်လိုက်သည်။

---

## 💡 Notes for Evaluation (စစ်ဆေးသူများအတွက်)
- ဒီ project မှာ `libft` ကို helper library အနေနဲ့သုံးထားပါတယ်။
- Norminette စည်းကမ်းများနှင့်အညီ ရေးသားထားပါတယ်။
- Memory leaks မရှိအောင် `free` လုပ်ခြင်းများ ထည့်သွင်းစဉ်းစားထားပါတယ်။ (သို့သော် `readline` ၏ reachable leaks အချို့ရှိနိုင်သည်ဟု subject ကဆိုထားသည်)

---

Minishell ရေးသားရာတွင် အခက်ခဲဆုံးမှာ **Process** များနှင့် **File Descriptor** များ စီမံခန့်ခွဲခြင်းဖြစ်သည်။ `fork` နှင့် `pipe` သဘောတရားကို သေချာနားလည်မှ အဆင်ပြေပါလိမ့်မည်။
