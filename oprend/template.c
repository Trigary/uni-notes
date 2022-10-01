//A -lrt kapcsoló használata szükséges fordításkor

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <poll.h>
#include <mqueue.h>
#include <sys/select.h>
#include <sys/msg.h>
#include <sched.h>
#include <pwd.h>
#include <dirent.h>
#include <ctype.h>

#define SIGNAL_GENERAL SIGRTMIN

void signal_noop_handler(int signo, siginfo_t* info, void* context) {}

void signal_register_noop_handler() {
	struct sigaction sigact;
	sigact.sa_sigaction = signal_noop_handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	if (sigaction(SIGNAL_GENERAL, &sigact, NULL) != 0) {
		perror("Nem sikerült noop handler regisztrálása");
		exit(1);
	}
}

void signal_modify_mask(int how) {
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGNAL_GENERAL);
	if (sigprocmask(how, &set, NULL) != 0) {
		perror("Hiba szignál maszk beállítása során");
		exit(1);
	}
}

void signal_send(pid_t target, int payload) {
	sigval_t sigv;
	sigv.sival_ptr = 0; //zero-initialization: make Valgrind warning disappear
	sigv.sival_int = payload;
	if (sigqueue(target, SIGNAL_GENERAL, sigv) != 0) {
		perror("Nem sikerült a szignál küldése");
		exit(1);
	}
}

int signal_wait() {
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGNAL_GENERAL);
	
	siginfo_t info;
	if (sigwaitinfo(&set, &info) == -1) {
		perror("Nem sikerült a szignál fogadás");
		exit(1);
	} else {
		return info.si_value.sival_int;
	}
}

int file_open(char* path, int flags) {
	int file = open(path, flags | O_CREAT, 0666);
	if (file == -1) {
		perror("Nem sikerült megnyitni a fájlt/pipe-ot");
		exit(1);
	}
	return file;
}

void file_close(int file) {
	int result = close(file);
	if (result == -1) {
		perror("Nem sikerült bezárni a fájlt/pipe-ot");
		exit(1);
	}
}

void file_write(int file, void* data, size_t size) {
	size_t result = write(file, data, size);
	if (result == -1) {
		perror("Hiba fájlba/pipe-ba írás során");
		exit(1);
	} else if (result != size) {
		printf("Hiba fájlba/pipe-ba írás során\n");
		exit(1);
	}
}

void file_read(int file, void* data, size_t size) {
	size_t result = read(file, data, size);
	if (result == -1) {
		perror("Hiba fájlból/pipe-ból olvasás során");
		exit(1);
	} else if (result != size) {
		printf("Hiba fájlból/pipe-ból olvasás során\n");
		exit(1);
	}
}

FILE* text_open(char* path, char* mode) {
	FILE* file = fopen(path, mode);
	if (file == NULL) {
		perror("Nem sikerült megnyitni a szöveges fájlt");
		exit(1);
	}
	return file;
}

void text_close(FILE* file) {
	int result = fclose(file);
	if (result == EOF) {
		perror("Nem sikerült bezárni a szöveges fájlt");
		exit(1);
	}
}

void text_write(int fprintf_return_value) {
	if (fprintf_return_value < 0) {
		perror("Hiba szöveges fájlba írás során");
		exit(1);
	}
}

void text_read(FILE* file, char* buffer, int chars_to_read_including_newline) {
	if (fgets(buffer, chars_to_read_including_newline, file) == NULL) {
		perror("Hiba szöveges fájlból olvasás során");
		exit(1);
	}
}

typedef struct {
	int read;
	int write;
} pipes_t;

pipes_t pipe_create() {
	int tmp[2];
	if (pipe(tmp) != 0) {
		perror("Nem sikerült elkészíteni a csövet");
		exit(1);
	}
	pipes_t pipes;
	pipes.read = tmp[0];
	pipes.write = tmp[1];
	return pipes;
}

pid_t child_fork() {
	pid_t pid = fork();
	if (pid == -1) {
		perror("Nem sikerült a fork");
		exit(1);
	}
	return pid;
}

void child_wait_for_termination() {
	if (wait(NULL) == -1) {
		perror("Hiba a gyerek terminálására való várakozás során");
		exit(1);
	}
}

void random_init() {
	srand(getpid() ^ time(NULL));
}

int random_get(int minInclusive, int maxExclusive) {
	return (rand() % (maxExclusive - minInclusive)) + minInclusive;
}

typedef struct {
	int value; //TODO
} message_payload_t;

typedef struct {
	long type;
	message_payload_t payload;
} message_t;

int message_create(key_t key) {
	int queue = msgget(key, IPC_CREAT | 0666);
	if (queue == -1) {
		perror("Hiba üzenetsor létrehozása során");
		exit(1);
	}
	return queue;
}

int message_connect(key_t key) {
	int queue = msgget(key, 0);
	if (queue == -1) {
		perror("Hiba üzenetsorhoz csatlakozás során");
		exit(1);
	}
	return queue;
}

void message_delete(int queue) {
	if (msgctl(queue, IPC_RMID, NULL) != 0) {
		perror("Hiba üzenetsor törlése során");
		exit(1);
	}
}

void message_send(int queue, int type, message_payload_t payload) {
	if (type <= 0) {
		printf("Érvénytelen üzenet típus: %d\n", type);
		exit(1);
	}
	
	message_t message;
	message.type = type;
	message.payload = payload;
	if (msgsnd(queue, &message, sizeof(message.payload), 0) != 0) {
		perror("Hiba üzenetsorba küldés során");
		exit(1);
	}
}

message_t message_receive(int queue, int type) {
	message_t message;
	ssize_t res = msgrcv(queue, &message, sizeof(message.payload), type, 0);
	if (res == -1) {
		perror("Hiba üzenetsorból beolvasás során");
		exit(1);
	} else if (res != sizeof(message.payload)) {
		printf("Hiba üzenetsorból beolvasás során\n");
		exit(1);
	}
	return message;
}

int memory_create(key_t key, size_t size) {
	int memory = shmget(key, size, IPC_CREAT | 0666);
	if (memory == -1) {
		perror("Hiba osztott memória létrehozása során");
		exit(1);
	}
	return memory;
}

int memory_connect(key_t key, size_t size) {
	int memory = shmget(key, size, 0);
	if (memory == -1) {
		perror("Hiba osztott memória csatlakozás során");
		exit(1);
	}
	return memory;
}

void memory_delete(int memory) {
	if (shmctl(memory, IPC_RMID, NULL) != 0) {
		perror("Hiba osztott memória törlése során");
		exit(1);
	}
}

void* memory_attach(int memory) {
	void* address = shmat(memory, NULL, 0);
	if (address == (void*) -1) {
		perror("Hiba osztott memória cím lekérdezés során");
		exit(1);
	}
	return address;
}

void memory_detach(const void* address) { //Nem történik semmi baj, ha nem csinálom meg
	if (shmdt(address) != 0) {
		perror("Hiba osztott memóriáról leválás során");
		exit(1);
	}
}

int semaphore_create(key_t key, int initial_value) {
	int semaphores = semget(key, 1, IPC_CREAT | 0666);
	if (semaphores == -1) {
		perror("Hiba szemafor készlet létrehozása során");
		exit(1);
	}
	if (semctl(semaphores, 0, SETVAL, initial_value) != 0) {
		perror("Hiba szemafor készlet inicializálása során");
		exit(1);
	}
	return semaphores;
}

int semaphore_connect(key_t key) {
	int semaphores = semget(key, 1, 0);
	if (semaphores == -1) {
		perror("Hiba szemafor készlethez csatlakozás során");
		exit(1);
	}
	return semaphores;
}

void semaphore_delete(int semaphores) {
	if (semctl(semaphores, 0, IPC_RMID) != 0) {
		perror("Hiba szemafor készlet törlése során");
		exit(1);
	}
}

void semaphore_change(int semaphores, short delta) {
	struct sembuf operations[1];
	operations[0].sem_num = 0;
	operations[0].sem_flg = 0;
	operations[0].sem_op = delta;
	if (semop(semaphores, operations, 1) != 0) {
		perror("Hiba szemafor változtatása során");
		exit(1);
	}
}

char* datetime_date() {
	static char buffer[50];
	time_t current = time(NULL);
	struct tm* local = localtime(&current);
	buffer[0] = '\0';
	sprintf(buffer, "%d-%d-%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
	return buffer;
}

char* datetime_time() {
	static char buffer[50];
	time_t current = time(NULL);
	struct tm* local = localtime(&current);
	buffer[0] = '\0';
	sprintf(buffer, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
	return buffer;
}

char* datetime_date_and_time() {
	static char buffer[100];
	time_t current = time(NULL);
	struct tm* local = localtime(&current);
	buffer[0] = '\0';
	sprintf(buffer, "%d-%d-%d %d:%d:%d",
			local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
			local->tm_hour, local->tm_min, local->tm_sec);
	return buffer;
}

//TODO esetleg: timer, fájl zárolás, pollolás

//////////////////////////////
// END OF UTILITY FUNCTIONS //
//////////////////////////////



int main(int argc, char** argv) { //A -lrt kapcsoló használata szükséges fordításkor
	return 0;
}
