package com.example;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File; // Import the File class
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONObject;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.TelegramBotsApi;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.updatesreceivers.DefaultBotSession;


public class App extends TelegramLongPollingBot {

    private static String BOT_TOKEN;
    private static final String BOT_USERNAME = "AI Zed Bot";
    private static final String TOKENS_FILE_PATH = ".env/tokens.txt";
    private static final String LM_STUDIO_API_URL = "http://127.0.0.1:8080/v1/chat/completions";


    private final Map<Long, String> userModels; // Stores the selected model for each user

    public App() {
        userModels = new HashMap<>();
    }

    public static void main(String[] args) {
        try {
            BOT_TOKEN = readTokenFromFile(TOKENS_FILE_PATH);
            TelegramBotsApi botsApi = new TelegramBotsApi(DefaultBotSession.class);
            botsApi.registerBot(new App());
            printLogHeader("BOT STARTED", "Bot is now running and waiting for messages!");
        } catch (TelegramApiException e) {
            printLogError("BOT STARTUP FAILED", e.getMessage());
            e.printStackTrace();
        } catch (IOException e) {
            printLogError("TOKEN READ ERROR", "Error reading token from file: " + e.getMessage());
            System.exit(1);
        }
    }

    private static String readTokenFromFile(String filePath) throws IOException {
        Path path = Paths.get(filePath);
        List<String> lines = Files.readAllLines(path);

        if (lines.isEmpty()) {
            throw new IOException("tokens.txt file is empty.");
        }

        return lines.get(0).trim();
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
        if (update.hasMessage() && update.getMessage().hasText()) {
            String messageText = update.getMessage().getText();
            long chatId = update.getMessage().getChatId();

            printLog("MESSAGE RECEIVED", "Chat ID: " + chatId, "Content: " + messageText);

            if (messageText.startsWith("/start")) {
                sendStartMessage(chatId);
            } else if (messageText.startsWith("/model")) {
                handleModelSelection(chatId, messageText);
            } else {
                String selectedModel = userModels.getOrDefault(chatId, "codestral-22b-v0.1");
                printLog("MODEL SELECTION", "Chat ID: " + chatId, "Model: " + selectedModel);

                String responseText = sendToLMStudio(messageText, selectedModel);

                SendMessage message = new SendMessage();
                message.setChatId(String.valueOf(chatId));
                message.setText(responseText);

                try {
                    execute(message);
                    printLog("RESPONSE SENT", "Chat ID: " + chatId, "Status: Success");
                } catch (TelegramApiException e) {
                    printLogError("RESPONSE ERROR", "Error sending response to user: " + e.getMessage());
                    e.printStackTrace();
                }
            }
        }
    }

    private void sendStartMessage(long chatId) {
        SendMessage message = new SendMessage();
        message.setChatId(String.valueOf(chatId));
        message.setText("Bot is running and ready to use! Use the /model command to select a model.");

        try {
            execute(message);
            printLog("START MESSAGE", "Chat ID: " + chatId, "Status: Sent");
        } catch (TelegramApiException e) {
            printLogError("START MESSAGE ERROR", "Error sending start message: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void handleModelSelection(long chatId, String messageText) {
        String[] parts = messageText.split(" ");
        if (parts.length == 2) {
            String model = parts[1];
            String fullModel = getFullModelName(model);

            if (fullModel != null) {
                userModels.put(chatId, fullModel);
                SendMessage message = new SendMessage();
                message.setChatId(String.valueOf(chatId));
                message.setText("You selected model: " + model);

                try {
                    execute(message);
                    printLog("MODEL CHANGED", "Chat ID: " + chatId, "Model: " + model);
                } catch (TelegramApiException e) {
                    printLogError("MODEL SELECTION ERROR", "Error sending confirmation: " + e.getMessage());
                    e.printStackTrace();
                }
            } else {
                sendModelSelectionHelp(chatId);
            }
        } else {
            sendModelSelectionHelp(chatId);
        }
    }

    private String getFullModelName(String modelAlias) {
        switch (modelAlias) {
            case "codestral":
                return "codestral-22b-v0.1";
            case "mathstral":
                return "mathstral-7b-v0.1";
            default:
                return null;
        }
    }

    private void sendModelSelectionHelp(long chatId) {
        SendMessage message = new SendMessage();
        message.setChatId(String.valueOf(chatId));
        message.setText("Please select a model using the command /model codestral or /model mathstral");

        try {
            execute(message);
            printLog("MODEL HELP", "Chat ID: " + chatId, "Status: Sent");
        } catch (TelegramApiException e) {
            printLogError("MODEL HELP ERROR", "Error sending help: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private String sendToLMStudio(String messageText, String model) {
        printLogHeader("LM STUDIO REQUEST", "Starting request to LM Studio API");
    
        try {
            JSONObject requestJson = new JSONObject();
            requestJson.put("model", model);
    
            JSONArray messagesArray = new JSONArray();
    
            JSONObject userMessage = new JSONObject();
            userMessage.put("role", "user");
            userMessage.put("content", messageText);
            messagesArray.put(userMessage);
    
            requestJson.put("messages", messagesArray);
    
            String requestBody = requestJson.toString();
            printLog("REQUEST BODY", "Length: " + requestBody.length() + " chars", "Content: " +
                   (requestBody.length() > 100 ? requestBody.substring(0, 100) + "..." : requestBody));
    
            // 1. Create a temporary file to store the JSON payload
            Path tempFile = Files.createTempFile("request", ".json");
            Files.write(tempFile, requestBody.getBytes("UTF-8"));  // Ensure UTF-8 encoding
            String tempFilePath = tempFile.toAbsolutePath().toString();
    
            // 2. Construct the curl command using the temporary file
            String command = String.format("curl -X POST -H \"Content-Type: application/json\" -H \"Accept: application/json\" --data \"@\\\"%s\\\"\" %s",
                    tempFilePath, LM_STUDIO_API_URL);
    
            printLog("CURL COMMAND", "Command: ", command);
    
            // 3. Execute the curl command
            ProcessBuilder processBuilder = new ProcessBuilder("cmd", "/c", command);
            processBuilder.redirectErrorStream(true);
    
            Process process = processBuilder.start();
    
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            StringBuilder output = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                output.append(line).append('\n');
            }
    
            int exitCode = process.waitFor();
    
            printLog("CURL EXECUTION", "Exit Code: " + exitCode, "");
    
            // 4. Delete the temporary file
            Files.deleteIfExists(tempFile);
    
            if (exitCode == 0) {
                String responseBody = output.toString();
                String logResponseBody = responseBody.length() > 300 ? responseBody.substring(0, 300) + "... [truncated]" : responseBody;
    
                printLog("RESPONSE RECEIVED", "Body Length: " + responseBody.length() + " chars", "");
                printLog("RESPONSE BODY", "Content: " + logResponseBody, "");
    
                try {
                    JSONObject jsonResponse = new JSONObject(responseBody);
                    JSONArray choices = jsonResponse.getJSONArray("choices");
                    JSONObject firstChoice = choices.getJSONObject(0);
                    JSONObject message = firstChoice.getJSONObject("message");
                    String content = message.getString("content");
    
                    printLogHeader("LM STUDIO REQUEST", "Request completed successfully");
                    return content;
    
                } catch (Exception e) {
                    printLogError("JSON PARSING ERROR", "Error parsing JSON response: " + e.getMessage());
                    printLogError("RESPONSE BODY", "Content: " + responseBody);
                    printLogHeader("LM STUDIO REQUEST", "Request failed due to JSON parsing exception");
                    return "Sorry, I encountered an error parsing the response from the server.";
                }
    
            } else {
                String errorMessage = "CURL ERROR: Exit Code " + exitCode + ", Output: " + output.toString();
                printLogError("API ERROR", errorMessage);
                printLogHeader("LM STUDIO REQUEST", "Request failed with exit code " + exitCode);
                return "Sorry, I encountered an error when processing your request. Error code: " + exitCode;
            }
    
        } catch (Exception e) {
            printLogError("GENERAL ERROR", "Error with request: " + e.getMessage());
            e.printStackTrace();
            printLogHeader("LM STUDIO REQUEST", "Request failed due to exception");
            return "Sorry, an error occurred while processing your request. Please try again later.";
        }
    }

    // Improved logging methods to create table-like output
    private static void printLogHeader(String type, String message) {
        String border = "+=".repeat(40) + "+";
        System.out.println("\n" + border);
        System.out.println("| " + type + ": " + message);
        System.out.println(border);
    }

    private static void printLog(String type, String detail1, String detail2) {
        String line = "| " + padRight(type, 20) + "| " + padRight(detail1, 30) + "| " + detail2;
        System.out.println(line);
        System.out.println("+--------------------+------------------------------+----------------------------------------+");
    }

    private static void printLogError(String type, String errorMessage) {
        String border = "+".repeat(81);
        System.out.println("\n" + border);
        System.out.println("| ERROR: " + type);
        System.out.println("| " + errorMessage);
        System.out.println(border + "\n");
    }

    private static String padRight(String s, int n) {
        return String.format("%-" + n + "s", s);
    }
}
