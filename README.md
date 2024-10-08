<p align="center">
  <a href="https://artnow.ru/kartina-Evgeniy-Onegin-Portret-hudozhnik-Shipitsova-Elena-998492.html" rel="noopener">
 <img width=400 height=500 src="https://www.iconpacks.net/icons/1/free-stack-icon-965-thumb.png" alt="Project logo"></a>
</p>

<h1 align="center">Stack - "Last In First Out" Struct 📚</h1>

<h2 align="center">⚔️ Protection With Canaries And DJB2 Hash Function ⚔️</h2>

## 📝 Table of Contents
- [About](#about)
- [Getting Started](#getting_started)
- [Documentation](#documentation)
- [Stack Dump](#dump)
- [Example Usage](#usage)
- [Built Using](#built_using)
- [TODO List](#todo)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>

A stack is a data structure that is an ordered set of elements, in which the addition of new elements and the removal of existing ones is done from one end, called the top of the stack. Moreover, the first element to be removed from the stack is the one that was placed there last, that is, the “last-in, first-out” (LIFO) strategy is implemented on the stack.

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development.

### Clone Repository

What things you need to clone the repository to your own computer and .

```bash
git clone git@github.com:lvbealr/Stack.git
cd Stack/
git switch privateStack
```

### Build and Using

First of all, clone submodule <b>CustomWarning</b>.

```bash
git clone git@github.com:lvbealr/customWarning.git
```

You can use <b>MAKE</b> to build the program and run it.

Enter the following command in the terminal:

```bash
make
```

Once compiled you can run the executable with the following command.

```bash
./Stack
```

The program will start in manual mode and ask you to enter data for correct operation.

## 🗎 Documentation <a name = "documentation"></a>

For convenient use of the program, [Doxygen documentation](vk.com/lvbealr) is included with the project.

## 🔧 Stack Dump <a name = "dump"></a>

Dump files are provided for the stack, which are automatically generated into an .html file and stored in the dumps/ folder.

They contain complete information about the initialization of the stack, the memory it occupies, the values ​​​​inside the stack and other information!

<p align="center">
 <img width = 500 height = 500 src = "https://i.imgur.com/94EDE5r.png">
</p>



## 🎈 Example Usage <a name="usage"></a>

### DEFAULT MODE
Use stackPush() and stackPop() functions to interact with the stack!
```c
stackPush(Stack,     5);
stackPush(Stack,   10);
StackPush(Stack,   15);
StackPush(Stack,   20);
StackPush(Stack, 999);
stackPop  (Stack,   &x);

// [5, 10, 15, 20, 999] -> [5, 10, 15, 20]
```
## ⛏️ Built Using <a name = "built_using"></a>
- [customWarning](https://github.com/lvbealr/customWarning) - Warning Module

## ⌛ TODO List <a name = "todo"></a>
- [] Write CustomRealloc


## ✍️ Authors <a name = "authors"></a>

- [@lvbealr](https://github.com/lvbealr) - Idea & Initial work



## 🎉 Acknowledgements <a name = "acknowledgement"></a>

- [@livlavr](https://github.com/livlavr) - Good Boy
- [@Iprime111](https://github.com/Iprime111) - Bad Boy 😈