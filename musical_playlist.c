#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <stdbool.h>
// Node structure of each song
typedef struct SongNode {
    char name[100];
    struct SongNode *next;
} SongNode;

// Playlist structure
typedef struct {
    SongNode *head;
    SongNode *tail;
    SongNode *current;
    char likedSongs[100][100];  // Store liked songs
    int likedCount;             // Number of liked songs
    bool isPaused;              // Track play/pause state
} playlist;

// Function prototypes
playlist *createPlaylist();
SongNode *createSongNode(const char *title);
void addSong(playlist *playlist, const char*name);
void removeSong(playlist* playlist, const char*name);
void playNext(playlist*playlist);
void playPrevious(playlist *playlist);
void playRandom(playlist *playlist);
void playByIndex(playlist *playlist, int index);
void displayCurrentSong(playlist *playlist);
void likeSong(playlist *playlist);
void unlikeSong(playlist *playlist);
void displayPlaylist(playlist *playlist);
void displayLikedSongs(playlist *playlist);
void pauseOrPlay(playlist *playlist);
int getPlaylistSize(playlist *playlist);
// Create a new playlist
playlist *createPlaylist() {
    playlist *newplaylist = (playlist *) malloc (sizeof(playlist));
    newplaylist->head = newplaylist ->tail = newplaylist ->current = NULL;
    newplaylist-> likedCount = 0;
    newplaylist-> isPaused = false;
    return newplaylist;
}
// Create a new song node
SongNode *createSongNode(const char *title) {
    SongNode *newNode = (SongNode *) malloc(sizeof(SongNode));
    strcpy(newNode -> name, title);
    newNode -> next = NULL;
    return newNode;
}
// Add a song to the playlist
void addSong(playlist *playlist, const char *name) {
    SongNode *newNode = createSongNode(name);
    if (playlist -> head == NULL) {
        playlist -> head = playlist -> tail = newNode;
        playlist -> tail -> next = playlist -> head;
    } else {
        playlist -> tail -> next = newNode;
        playlist -> tail = newNode;
        playlist -> tail -> next = playlist -> head;
    }
    printf("Song '%s' added to the playlist\n" , name);
}
// Remove a song from the playlist
void removeSong(playlist *playlist, const char *name) {
    if (playlist -> head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    SongNode *current = playlist -> head, *prev = NULL;
    do {
        if (strcmp(current -> name, name) == 0) {
            if(current == playlist->head) {
                playlist->head = playlist -> head -> next;
                playlist ->tail->next = playlist -> head;
            }else if(current == playlist->tail) {
                playlist->tail = prev;
                playlist-> tail-> next = playlist -> head;
            }else {
                prev -> next = current ->next;
            }
            free(current);
            printf("Song '%s'removed from the playlist. \n ", name);
            return;
        }
        prev = current;
        current = current -> next;
    }while(current!=playlist->head);
    printf("Song'%s'not found in the playlist.\n",name);
}

// Play the next song
void playNext(playlist *playlist) {
    if(playlist ->current == NULL){
        playlist -> current = playlist -> head;
    }
    else{
        playlist -> current = playlist -> current ->next;
    }
    displayCurrentSong(playlist);
}
//play the previous song
void playPrevious(playlist*playlist) {
    if(playlist -> head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    SongNode* temp = playlist ->head;
    while (temp ->next !=playlist -> current) {
        temp = temp -> next;
    }
    playlist -> current = temp;
    displayCurrentSong(playlist);
}
// Play a random song
void playRandom(playlist *playlist) {
    int size = getPlaylistSize(playlist);
    int randomIndex = rand() % size;
    playByIndex(playlist, randomIndex);
}
// Play song by index
void playByIndex(playlist *playlist, int index) {
    SongNode *temp = playlist -> head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp -> next;
    }
    playlist -> current = temp;
    displayCurrentSong(playlist);
}
// Display the current song
void displayCurrentSong(playlist *playlist) {
    if (playlist -> current == NULL) {
        printf("No song is playing.\n");
    } else {
        printf("Now playing: %s%s\n", playlist -> current -> name,
               playlist -> isPaused ? " (Paused)" : "");
    }
}

// Like the current song
void likeSong(playlist *playlist) {
    if (playlist -> current == NULL) {
        printf("No song is playing to like.\n");
        return;
    }
    strcpy(playlist -> likedSongs[playlist -> likedCount], playlist -> current -> name);
    playlist -> likedCount++;
    printf("Song '%s' added to liked songs.\n", playlist -> current -> name);
}

// Unlike the current song
void unlikeSong(playlist *playlist) {
    if (playlist -> current == NULL) {
        printf("No song is playing to unlike.\n");
        return;
    }
for (int i = 0; i < playlist -> likedCount; i++) {
        if (strcmp(playlist -> likedSongs[i], playlist -> current -> name) == 0) {
            for (int j = i; j < playlist -> likedCount - 1; j++) {
                strcpy(playlist -> likedSongs[j], playlist -> likedSongs[j + 1]);
            }
            playlist -> likedCount--;
            printf("Song '%s' removed from liked songs.\n", playlist -> current -> name);
            return;
        }
    }
    printf("Song '%s' is not in liked songs.\n", playlist->current->name);
}

// Display all songs in the playlist
void displayPlaylist(playlist *playlist) {
    if (playlist->head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    SongNode *temp = playlist->head;
    do {
        printf("%s\n", temp->name);
        temp = temp->next;
    } while (temp != playlist->head);
}

// Display liked songs
void displayLikedSongs(playlist *playlist) {
    if (playlist->likedCount == 0) {
        printf("No liked songs.\n");
        return;
    }
    printf("Liked Songs:\n");
    for (int i = 0; i < playlist->likedCount; i++) {
        printf("%s\n", playlist->likedSongs[i]);
    }
}

// Pause or play the current song
void pauseOrPlay(playlist *playlist) {
    if (playlist->current == NULL) {
        printf("No song is playing.\n");
        return;
    }
    playlist->isPaused = !playlist->isPaused;
    displayCurrentSong(playlist);
}

// Get the size of the playlist
int getPlaylistSize(playlist *playlist) {
    if (playlist->head == NULL) return 0;
    int count = 0;
    SongNode *temp = playlist->head;
    do {
        count++;
        temp = temp->next;
    } while (temp != playlist->head);
    return count;
}

// Main function
int main() {
    playlist *myPlaylist = createPlaylist();
    int selection;
    bool running = true;
    srand(time(NULL));

    while (running) {
        printf("\nPlaylist Manager\n");
        printf("1.Add Song\n");
        printf("2.Delete Song\n");
        printf("3.Play next Song\n");
        printf("4.Play previous Song\n");
        printf("5.Play/pause current Song\n");
        printf("6.Shuffle current Song\n");
        printf("7.Play Song by index\n");
        printf("8.Like current Song\n");
        printf("9.Unlike current Song\n");
        printf("10.Show playlist\n");
        printf("11.Show liked Songs\n");
        printf("12.Exit\n");
        printf("Select an option: ");
        scanf("%d", &selection);
        getchar();  // Consume newline

        switch (selection) {
            case 1: {
                char title[100];
                printf("Enter song name: ");
                fgets(title, 100, stdin);
                title[strcspn(title, "\n")] = 0;
                addSong(myPlaylist, title);
                break;
            }
            case 2: {
                char title[100];
                printf("Enter song name to delete: ");
                fgets(title, 100, stdin);
                title[strcspn(title, "\n")] = 0;
                removeSong(myPlaylist, title);
                break;
            }
            case 3:
                playNext(myPlaylist);
                break;
            case 4:
                playPrevious(myPlaylist);
                break;
            case 5:
                pauseOrPlay(myPlaylist);
                break;
            case 6:
                playRandom(myPlaylist);
                break;
            case 7: {
                int index;
                printf("Enter song index: ");
                scanf("%d", &index);
                playByIndex(myPlaylist, index);
                break;
            }
            case 8:
                likeSong(myPlaylist);
                break;
            case 9:
                unlikeSong(myPlaylist);
                break;
            case 10:
                displayPlaylist(myPlaylist);
                break;
            case 11:
                displayLikedSongs(myPlaylist);
                break;
            case 12:
                running = false;
                break;
            default:
                printf("Invalid selection.\n");
        }
    }
    printf("Exiting Playlist Manager.\n");
return 0;
}
