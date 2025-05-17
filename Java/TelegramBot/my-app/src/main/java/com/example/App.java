package com.example;

import java.io.IOException;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.TelegramBotsApi;
//import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.updatesreceivers.DefaultBotSession;

public class App extends TelegramLongPollingBot {

    private static String BOT_TOKEN;
    private static final String BOT_USERNAME = "AI Zed Bot";
    private static final String TOKENS_FILE_PATH = ".env/tokens.txt";
//    private static final String LM_STUDIO_API_URL = "http://127.0.0.1:8080/v1/chat/completions";


    public App() {}

    public static void main(String[] args) {
        try {
            BOT_TOKEN = Utils.readTokenFromFile(TOKENS_FILE_PATH);
            TelegramBotsApi botsApi = new TelegramBotsApi(DefaultBotSession.class);
            botsApi.registerBot(new App());
            Utils.printLogHeader("BOT STARTED", "Bot is now running and waiting for messages!");
        } catch (TelegramApiException e) {
            Utils.printLogError("BOT STARTUP FAILED", e.getMessage());
            e.printStackTrace();
        } catch (IOException e) {
            Utils.printLogError("TOKEN READ ERROR", "Error reading token from file: " + e.getMessage());
            System.exit(1);
        }
    }

    @Override
    public String getBotUsername() {
        return BOT_USERNAME;
    }

    @Override
    public String getBotToken() {
        return BOT_TOKEN;
    }

    @Override
    public void onUpdateReceived(Update update) {
        Utils.handleUpdate(update, this);
    }
}
