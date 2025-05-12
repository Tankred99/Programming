def get_name():
    name = input("Введите ваше имя: ")
    return name

def hello_user(name: str) -> None:
    print(f"Привет, {name}!")
    print(f"Рады видеть вас, {name}!")
    print("Добро пожаловать в нашу программу!")

def main():
    user_name = get_name()
    hello_user(user_name)

if __name__ == "__main__":
    main()
