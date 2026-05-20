// ============================================================
//  CosmicFighter_All.cpp  --  Levels 1, 2, 3, and 4 combined
//
//  Controls (all levels):
//    Left / Right Arrow  -- move player
//    SPACE               -- fire
//    [1]  -- start Level 1
//    [2]  -- start Level 2
//    [3]  -- start Level 3
//    [4]  -- start Level 4
//    [b]  -- back to menu
//
//  Level 3 extras:  [Up Arrow] single shot | [S] shield
//  Level 4 extras:  [T] toggle day/night   | [R] restart
// ============================================================


#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1416f

// ── Top-level Game States ─────────────────────────────────────
#define STATE_MENU      0
#define STATE_LEVEL1    1
#define STATE_LEVEL2    2
#define STATE_LEVEL3    3
#define STATE_LEVEL4    4
#define STATE_GAMEOVER  5

int gameState = STATE_MENU;

// ── Shared star field (Levels 1 & 2) ─────────────────────────
float starX[200], starY[200];
float worldSpeed = 0.004f;

// =============================================================
//  LEVEL 1  globals
// =============================================================
float moonX       = -1.2f;
GLfloat moonAngle = 0.0f;
float moonScale   = 1.0f;
float scaleSpeed  = 0.0009f;

int nightFlag = 1, dayFlag = 0;
int score     = 0;
int life      = 3;

int enemyLife1    = 10;
int enemyMaxLife1 = 10;
int levelClear    = 0;

int asteroidTotalCreated = 0;
int levelTriggered       = 0;
int enemyActive1         = 0;
int asteroidsDestroyed   = 0;
#define MAX_ASTEROIDS_TOTAL 40

#define L1_MAX_ENEMY_BULLETS 10
float l1_eBulletX[L1_MAX_ENEMY_BULLETS];
float l1_eBulletY[L1_MAX_ENEMY_BULLETS];
int   l1_eBulletActive[L1_MAX_ENEMY_BULLETS];
float l1_eBulletSpeed = 0.03f;
int   l1_enemyShootTimer = 0;

float enemyX1 = 0.0f, enemyY1 = 0.75f;
float enemySpeed1 = 0.02f;
int   enemyDir1   = 1;

float rocketX = 0.0f, rocketY = -0.75f;
float speed1   = 0.05f;

float bulletX1 = 0.0f, bulletY1 = -0.75f;
int   bulletActive1 = 0;
float bulletSpeed1  = 0.05f;

#define L1_MAX_ASTEROIDS 20
float l1_asteroidX[L1_MAX_ASTEROIDS];
float l1_asteroidY[L1_MAX_ASTEROIDS];
float l1_asteroidSpeed[L1_MAX_ASTEROIDS];
int   l1_asteroidActive[L1_MAX_ASTEROIDS];

// =============================================================
//  LEVEL 2  globals
// =============================================================
GLfloat heroX = 0.0f, heroY = -0.75f;
GLfloat heroSpeed = 0.09f;
int     heroLife  = 4;
int     score2    = 0;

GLfloat bossX = 0.0f, bossY = 0.75f;
int     bossLife    = 15;
int     bossMaxLife = 15;

GLfloat heroBulletX[2], heroBulletY[2];
bool    isFiring = false;

GLfloat enemyBulletX2, enemyBulletY2;
bool    isEnemyFiring2 = false;
int     enemyCooldown  = 0;

#define MAX_TINY  14
float tinyX[MAX_TINY], tinyY[MAX_TINY];
int   tinyDir[MAX_TINY], tinyMoveCount[MAX_TINY];
bool  tinyActive[MAX_TINY];

#define MAX_STONES 30
float stoneX[MAX_STONES], stoneY[MAX_STONES];
float stoneAngle[MAX_STONES], stoneScale[MAX_STONES];
int   stoneDir[MAX_STONES];
bool  stoneActive[MAX_STONES];

bool gameOver2 = false;
bool gameWin2  = false;

// =============================================================
//  LEVEL 3  globals
// =============================================================
#define L3_STAR_COUNT     210
#define L3_MAX_PL_BULLETS  20
#define L3_MAX_EN_BULLETS  20
#define L3_MAX_POWERUPS    14

float l3_starX[L3_STAR_COUNT], l3_starY[L3_STAR_COUNT];

float l3_pBulletX[L3_MAX_PL_BULLETS];
float l3_pBulletY[L3_MAX_PL_BULLETS];
int   l3_pBulletActive[L3_MAX_PL_BULLETS];

float l3_eBulletX[L3_MAX_EN_BULLETS];
float l3_eBulletY[L3_MAX_EN_BULLETS];
int   l3_eBulletActive[L3_MAX_EN_BULLETS];

float l3_powerX[L3_MAX_POWERUPS];
float l3_powerY[L3_MAX_POWERUPS];
int   l3_powerActive[L3_MAX_POWERUPS];
int   l3_powerType[L3_MAX_POWERUPS];
float l3_powerAngle[L3_MAX_POWERUPS];

int   l3_gameState  = 0;   // 0=playing 1=win 2=lose (internal to L3)
int   l3_shieldActive = 0;
int   l3_shieldTimer  = 0;
int   l3_playerLife = 20;
int   l3_enemyLife  = 40;

float l3_normalX = 0.0f, l3_normalY = -0.75f;
float l3_enemyX  = 0.0f, l3_enemyY  =  0.75f;
float l3_enemyDX = 0.01f;
float l3_timeVar = 0.0f;

// =============================================================
//  LEVEL 4  globals
// =============================================================
#define L4_MAX_BULLETS   50
#define L4_MAX_ASTEROIDS  5

float l4_bulletX[L4_MAX_BULLETS],  l4_bulletY[L4_MAX_BULLETS];
bool  l4_bulletActive[L4_MAX_BULLETS];

float l4_pBulletX[L4_MAX_BULLETS], l4_pBulletY[L4_MAX_BULLETS];
bool  l4_pBulletActive[L4_MAX_BULLETS];

float l4_asteroidX[L4_MAX_ASTEROIDS];
float l4_asteroidY[L4_MAX_ASTEROIDS];
float l4_asteroidR[L4_MAX_ASTEROIDS];
bool  l4_asteroidActive[L4_MAX_ASTEROIDS];

bool  l4_dayMode  = false;
float l4_cloudX[5] = {-0.8f, -0.4f, 0.0f, 0.4f, 0.8f};
float l4_cloudY[5] = { 0.7f,  0.6f, 0.75f, 0.65f, 0.8f};

bool  l4_gameOver = false;
bool  l4_win      = false;

int   l4_enemyLife          = 15;
int   l4_enemyShootDelay    = 0;
int   l4_playerShootCounter = 0;
int   l4_playerLife         = 3;

float l4_normalX = 0.0f, l4_normalY = -0.75f;
float l4_enemyX  = 0.0f, l4_enemyY  =  0.80f;
float l4_enemyDX = 0.01f, l4_enemyDY = 0.005f;

float l4_starX[100], l4_starY[100];

// =============================================================
//  Forward declarations
// =============================================================
void drawMenu();
void level1();
void level2();
void level3Display();
void level4Display();
void drawGameOver();
void update(int value);
void specialKeys(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);

// ──────────────────────────────────────────────────────────────
//  SHARED helpers (Levels 1 & 2)
// ──────────────────────────────────────────────────────────────
void updateStars() {
    for (int i = 0; i < 200; i++) {
        starY[i] -= worldSpeed;
        if (starY[i] < -1.0f) {
            starY[i] = 1.0f;
            starX[i] = (rand() % 200 - 100) / 100.0f;
        }
    }
}

// Circle helper for L1 moon
void CC(float x, float y, float ra, int r, int g, int b) {
    int segs = 40;
    GLfloat tp2 = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(r, g, b);
    glVertex2f(x, y);
    glColor3ub(255, 242, 179);
    for (int i = 0; i <= segs; i++)
        glVertex2f(x + ra * cos(i * tp2 / segs),
                   y + ra * sin(i * tp2 / segs));
    glEnd();
}

// =============================================================
//  LEVEL 1  –  draw functions
// =============================================================
void backgroundLevel1Evening() {
    glColor3f(0.1f, 0.1f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-1, 1); glVertex2f(-1,-1);
        glVertex2f( 1,-1); glVertex2f( 1, 1);
    glEnd();
    glBegin(GL_POINTS); glColor3f(1,1,1);
    for (int i = 0; i < 200; i++) glVertex2f(starX[i], starY[i]);
    glEnd();
}

void backgroundLevel1Night() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1, 1); glVertex2f(-1,-1);
        glVertex2f( 1,-1); glVertex2f( 1, 1);
    glEnd();
    glBegin(GL_POINTS); glColor3f(1,1,1);
    for (int i = 0; i < 200; i++) glVertex2f(starX[i], starY[i]);
    glEnd();
}

void enemyRocketLevel1(float x, float y) {
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y-0.15f); glVertex2f(x-0.05f, y+0.1f); glVertex2f(x+0.05f, y+0.1f);
    glEnd();
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y-0.12f); glVertex2f(x-0.02f, y+0.05f); glVertex2f(x+0.02f, y+0.05f);
    glEnd();
    glColor3f(0.7f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.05f, y+0.05f); glVertex2f(x-0.15f, y+0.15f); glVertex2f(x-0.05f, y+0.15f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.05f, y+0.05f); glVertex2f(x+0.15f, y+0.15f); glVertex2f(x+0.05f, y+0.15f);
    glEnd();
    glColor3f(0.9f, 0.3f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.05f, y); glVertex2f(x-0.1f, y+0.05f); glVertex2f(x-0.05f, y+0.1f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.05f, y); glVertex2f(x+0.1f, y+0.05f); glVertex2f(x+0.05f, y+0.1f);
    glEnd();
    // Cockpit
    glColor3f(1.0f, 0.6f, 0.0f);
    float r = 0.02f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y-0.05f);
    for (int i = 0; i <= 30; i++) {
        float a = 2*PI*i/30;
        glVertex2f(x+r*cos(a), y-0.05f+r*sin(a));
    }
    glEnd();
    // Flame top
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.02f, y+0.1f); glVertex2f(x+0.02f, y+0.1f); glVertex2f(x, y+0.2f);
    glEnd();
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.01f, y+0.1f); glVertex2f(x+0.01f, y+0.1f); glVertex2f(x, y+0.17f);
    glEnd();
}

void normalRocket(float x, float y) {
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.03f, y-0.1f); glVertex2f(x+0.03f, y-0.1f);
        glVertex2f(x+0.03f, y+0.05f); glVertex2f(x-0.03f, y+0.05f);
    glEnd();
    glColor3f(1.0f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.03f, y+0.05f); glVertex2f(x+0.03f, y+0.05f); glVertex2f(x, y+0.12f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.03f, y-0.05f); glVertex2f(x-0.08f, y-0.12f); glVertex2f(x-0.03f, y-0.12f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.03f, y-0.05f); glVertex2f(x+0.08f, y-0.12f); glVertex2f(x+0.03f, y-0.12f);
    glEnd();
    glColor3f(0.2f, 0.6f, 1.0f);
    float r = 0.015f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 30; i++) {
        float a = 2*PI*i/30;
        glVertex2f(x+r*cos(a), y+r*sin(a));
    }
    glEnd();
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.02f, y-0.1f); glVertex2f(x+0.02f, y-0.1f); glVertex2f(x, y-0.18f);
    glEnd();
}

void bulletLevel1(float x, float y, float sz) {
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(x-sz, y-sz); glVertex2f(x+sz, y-sz);
        glVertex2f(x+sz, y+sz); glVertex2f(x-sz, y+sz);
    glEnd();
}

void asteroidLevel1(float x, float y) {
    glColor3f(0.5f, 0.5f, 0.5f);
    float r = 0.03f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = 2*PI*i/20;
        glVertex2f(x+r*cos(a), y+r*sin(a));
    }
    glEnd();
}

void enemyBulletLevel1(float x, float y) {
    glColor3f(1,1,0);
    glBegin(GL_QUADS);
        glVertex2f(x-0.008f, y-0.02f); glVertex2f(x+0.008f, y-0.02f);
        glVertex2f(x+0.008f, y+0.02f); glVertex2f(x-0.008f, y+0.02f);
    glEnd();
}

// =============================================================
//  LEVEL 1  –  logic
// =============================================================
void l1_initAsteroids() {
    for (int i = 0; i < L1_MAX_ASTEROIDS; i++) {
        l1_asteroidActive[i] = 1;
        l1_asteroidX[i]      = (rand()%200-100)/100.0f;
        l1_asteroidY[i]      = 1.0f+(rand()%100)/100.0f;
        l1_asteroidSpeed[i]  = 0.002f;
        asteroidTotalCreated++;
    }
}

void resetLevel1() {
    score = 0; life = 3;
    enemyLife1 = enemyMaxLife1;
    enemyActive1 = 0; levelTriggered = 0; levelClear = 0;
    asteroidTotalCreated = 0; asteroidsDestroyed = 0;
    bulletActive1 = 0; l1_enemyShootTimer = 0;
    rocketX = 0.0f; rocketY = -0.75f;
    moonX = -1.2f; moonScale = 1.0f; moonAngle = 0.0f;
    for (int i = 0; i < L1_MAX_ASTEROIDS; i++) {
        l1_asteroidActive[i] = 1;
        l1_asteroidX[i]      = (rand()%200-100)/100.0f;
        l1_asteroidY[i]      = 1.0f+(rand()%100)/100.0f;
        l1_asteroidSpeed[i]  = 0.002f;
    }
    for (int i = 0; i < L1_MAX_ENEMY_BULLETS; i++) l1_eBulletActive[i] = 0;
}

void updateBullet1() {
    if (bulletActive1) {
        bulletY1 += bulletSpeed1;
        if (bulletY1 > 1.0f) bulletActive1 = 0;
    }
}

void updateAsteroids1() {
    for (int i = 0; i < L1_MAX_ASTEROIDS; i++) {
        if (l1_asteroidActive[i]) {
            l1_asteroidY[i] -= l1_asteroidSpeed[i];
            if (l1_asteroidY[i] < -1.0f) {
                if (asteroidTotalCreated < MAX_ASTEROIDS_TOTAL) {
                    l1_asteroidY[i] = 1.0f;
                    l1_asteroidX[i] = (rand()%200-100)/100.0f;
                    asteroidTotalCreated++;
                } else {
                    l1_asteroidActive[i] = 0;
                }
            }
        }
    }
}

void checkAsteroidCollision1() {
    for (int i = 0; i < L1_MAX_ASTEROIDS; i++) {
        if (l1_asteroidActive[i] && bulletActive1) {
            if (fabs(bulletX1 - l1_asteroidX[i]) < 0.05f &&
                fabs(bulletY1 - l1_asteroidY[i]) < 0.05f) {
                l1_asteroidActive[i] = 0;
                bulletActive1 = 0;
                asteroidsDestroyed++;
                score += 10;
            }
        }
    }
}

void checkRocketAsteroidCollision1() {
    for (int i = 0; i < L1_MAX_ASTEROIDS; i++) {
        if (l1_asteroidActive[i]) {
            if (fabs(rocketX - l1_asteroidX[i]) < 0.07f &&
                fabs(rocketY - l1_asteroidY[i]) < 0.07f) {
                l1_asteroidActive[i] = 0;
                life--;
                rocketX = 0.0f; rocketY = -0.75f;
                if (life <= 0) gameState = STATE_GAMEOVER;
            }
        }
    }
}

void checkEnemyCollision1() {
    if (!enemyActive1 || !bulletActive1) return;
    if (fabs(bulletX1-enemyX1) < 0.1f && fabs(bulletY1-enemyY1) < 0.1f) {
        bulletActive1 = 0;
        enemyLife1--;
        score += 5;
    }
}

void enemyShoot1() {
    if (!enemyActive1) return;
    l1_enemyShootTimer++;
    if (l1_enemyShootTimer > 60) {
        l1_enemyShootTimer = 0;
        for (int i = 0; i < L1_MAX_ENEMY_BULLETS; i++) {
            if (!l1_eBulletActive[i]) {
                l1_eBulletActive[i] = 1;
                l1_eBulletX[i] = enemyX1;
                l1_eBulletY[i] = enemyY1;
                break;
            }
        }
    }
}

void updateEnemyBullets1() {
    for (int i = 0; i < L1_MAX_ENEMY_BULLETS; i++) {
        if (l1_eBulletActive[i]) {
            l1_eBulletY[i] -= l1_eBulletSpeed;
            if (l1_eBulletY[i] < -1.0f) { l1_eBulletActive[i] = 0; continue; }
            if (fabs(l1_eBulletX[i]-rocketX) < 0.05f &&
                fabs(l1_eBulletY[i]-rocketY) < 0.05f) {
                l1_eBulletActive[i] = 0;
                life--;
                rocketX = 0.0f; rocketY = -0.75f;
                if (life <= 0) gameState = STATE_GAMEOVER;
            }
        }
    }
}

void updateEnemy1() {
    if (!enemyActive1) return;
    enemyX1 += enemySpeed1 * enemyDir1;
    if (enemyX1 >  0.9f) enemyDir1 = -1;
    if (enemyX1 < -0.9f) enemyDir1 =  1;
    if (enemyLife1 <= 0) { enemyActive1 = 0; levelClear = 1; }
}

void level1() {
    if (asteroidsDestroyed < 10)
        backgroundLevel1Evening();
    else
        backgroundLevel1Night();

    glPushMatrix();
    glTranslatef(moonX, 0.75f, 0.0f);
    glRotatef(moonAngle, 0,0,1);
    glScalef(moonScale, moonScale, 1.0f);
    CC(0.0f, 0.0f, 0.15f, 255, 242, 179);
    glPopMatrix();

    if (enemyActive1) enemyRocketLevel1(enemyX1, enemyY1);
    normalRocket(rocketX, rocketY);
    if (bulletActive1) bulletLevel1(bulletX1, bulletY1, 0.01f);

    for (int i = 0; i < L1_MAX_ASTEROIDS; i++)
        if (l1_asteroidActive[i]) asteroidLevel1(l1_asteroidX[i], l1_asteroidY[i]);
    for (int i = 0; i < L1_MAX_ENEMY_BULLETS; i++)
        if (l1_eBulletActive[i]) enemyBulletLevel1(l1_eBulletX[i], l1_eBulletY[i]);

    // HUD
    glColor3f(1,1,1);
    glRasterPos2f(-0.95f, 0.9f);
    char scoreText[50]; sprintf(scoreText, "Score: %d", score);
    for (int i = 0; scoreText[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    glRasterPos2f(-0.95f, 0.8f);
    char lifeText[50]; sprintf(lifeText, "Life: %d", life);
    for (int i = 0; lifeText[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lifeText[i]);

    if (enemyActive1) {
        glColor3f(1,0,0);
        float barW = 0.4f * ((float)enemyLife1 / enemyMaxLife1);
        glBegin(GL_QUADS);
            glVertex2f(-0.2f,0.95f); glVertex2f(-0.2f+barW,0.95f);
            glVertex2f(-0.2f+barW,0.98f); glVertex2f(-0.2f,0.98f);
        glEnd();
    }

    if (levelClear) {
        glColor3f(0,1,0);
        glRasterPos2f(-0.25f, 0.2f);
        char msg[] = "CONGRATULATIONS!";
        for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
        glRasterPos2f(-0.35f, 0.1f);
        char msg2[] = "Press [2] for Level 2";
        for (int i = 0; msg2[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg2[i]);
    }
}

// =============================================================
//  LEVEL 2  –  draw functions
// =============================================================
void bossRocketLevel2(float x, float y) {
    glColor3f(0.5f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y-0.18f); glVertex2f(x-0.07f, y+0.12f); glVertex2f(x+0.07f, y+0.12f);
    glEnd();
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y-0.18f); glVertex2f(x-0.07f, y+0.12f); glVertex2f(x+0.07f, y+0.12f);
    glEnd();
    glColor3f(0.6f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.07f, y+0.05f); glVertex2f(x-0.18f, y+0.18f); glVertex2f(x-0.07f, y+0.18f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.07f, y+0.05f); glVertex2f(x+0.18f, y+0.18f); glVertex2f(x+0.07f, y+0.18f);
    glEnd();
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.11f, y-0.05f); glVertex2f(x-0.08f, y-0.05f);
        glVertex2f(x-0.08f, y+0.08f); glVertex2f(x-0.11f, y+0.08f);
        glVertex2f(x+0.08f, y-0.05f); glVertex2f(x+0.11f, y-0.05f);
        glVertex2f(x+0.11f, y+0.08f); glVertex2f(x+0.08f, y+0.08f);
    glEnd();
    float r = 0.025f;
    glColor3f(0,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y-0.03f);
    for (int i = 0; i <= 40; i++) {
        float a = 2*PI*i/40;
        glVertex2f(x+r*cos(a), y-0.03f+r*sin(a));
    }
    glEnd();
    r = 0.012f;
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y-0.03f);
    for (int i = 0; i <= 40; i++) {
        float a = 2*PI*i/40;
        glVertex2f(x+r*cos(a), y-0.03f+r*sin(a));
    }
    glEnd();
    float flicker = (rand()%10)/500.0f;
    glColor3f(1.0f, 0.4f, 0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.025f, y+0.12f); glVertex2f(x+0.025f, y+0.12f);
        glVertex2f(x, y+0.28f+flicker);
    glEnd();
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.012f, y+0.12f); glVertex2f(x+0.012f, y+0.12f);
        glVertex2f(x, y+0.22f+flicker);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.11f, y+0.08f); glVertex2f(x-0.08f, y+0.08f); glVertex2f(x-0.095f, y+0.20f);
        glVertex2f(x+0.08f, y+0.08f); glVertex2f(x+0.11f, y+0.08f); glVertex2f(x+0.095f, y+0.20f);
    glEnd();
}

void level2Rocket(float x, float y) {
    glColor3f(0.6f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.035f, y-0.12f); glVertex2f(x+0.035f, y-0.12f);
        glVertex2f(x+0.035f, y+0.07f); glVertex2f(x-0.035f, y+0.07f);
    glEnd();
    glColor3f(0.2f, 0.2f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.035f, y+0.07f); glVertex2f(x+0.035f, y+0.07f); glVertex2f(x, y+0.16f);
    glEnd();
    glColor3f(0.3f, 0.3f, 0.8f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.035f, y-0.05f); glVertex2f(x-0.10f, y-0.14f); glVertex2f(x-0.035f, y-0.14f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.035f, y-0.05f); glVertex2f(x+0.10f, y-0.14f); glVertex2f(x+0.035f, y-0.14f);
    glEnd();
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.06f, y-0.10f); glVertex2f(x-0.045f, y-0.10f);
        glVertex2f(x-0.045f, y+0.03f); glVertex2f(x-0.06f,  y+0.03f);
        glVertex2f(x+0.045f, y-0.10f); glVertex2f(x+0.06f,  y-0.10f);
        glVertex2f(x+0.06f,  y+0.03f); glVertex2f(x+0.045f, y+0.03f);
    glEnd();
    glColor3f(0.2f, 0.8f, 1.0f);
    float r = 0.012f;
    for (int j = 0; j < 2; j++) {
        float wy = y + j*0.04f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, wy);
        for (int i = 0; i <= 30; i++) {
            float a = 2*PI*i/30;
            glVertex2f(x+r*cos(a), wy+r*sin(a));
        }
        glEnd();
    }
    glColor3f(1.0f, 0.4f, 0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.02f, y-0.12f); glVertex2f(x+0.02f, y-0.12f); glVertex2f(x, y-0.22f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.055f, y-0.10f); glVertex2f(x-0.045f, y-0.10f); glVertex2f(x-0.05f, y-0.18f);
        glVertex2f(x+0.045f, y-0.10f); glVertex2f(x+0.055f, y-0.10f); glVertex2f(x+0.05f, y-0.18f);
    glEnd();
}

void tinyRocket(float x, float y) {
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y+0.03f); glVertex2f(x-0.02f, y-0.03f); glVertex2f(x+0.02f, y-0.03f);
    glEnd();
    glColor3f(0.2f, 0.8f, 1.0f);
    float r = 0.008f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float a = 2*PI*i/20;
        glVertex2f(x+r*cos(a), y+r*sin(a));
    }
    glEnd();
    glColor3f(1,0.3f,0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.02f, y-0.01f); glVertex2f(x-0.04f, y-0.03f); glVertex2f(x-0.02f, y-0.03f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(x+0.02f, y-0.01f); glVertex2f(x+0.04f, y-0.03f); glVertex2f(x+0.02f, y-0.03f);
    glEnd();
    glColor3f(1,0.5f,0);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.01f, y-0.03f); glVertex2f(x+0.01f, y-0.03f); glVertex2f(x, y-0.06f);
    glEnd();
}

void drawHeroBullet(float x, float y) {
    glColor3f(1,1,0);
    glBegin(GL_QUADS);
        glVertex2f(x-0.01f, y); glVertex2f(x+0.01f, y);
        glVertex2f(x+0.01f, y+0.05f); glVertex2f(x-0.01f, y+0.05f);
    glEnd();
}

void drawEnemyBullet2(float x, float y) {
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
        glVertex2f(x-0.01f, y); glVertex2f(x+0.01f, y);
        glVertex2f(x+0.01f, y-0.05f); glVertex2f(x-0.01f, y-0.05f);
    glEnd();
}

void background2() {
    bool blackout = (score2 >= 100);
    if (blackout) {
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
            glVertex2f(-1,-1); glVertex2f(1,-1); glVertex2f(1,1); glVertex2f(-1,1);
        glEnd();
    } else {
        glBegin(GL_QUADS);
            glColor3f(0,0,0); glVertex2f(-1,-1); glVertex2f(1,-1);
            glColor3f(0,0,0.25f); glVertex2f(1,1); glVertex2f(-1,1);
        glEnd();
    }
    glBegin(GL_POINTS); glColor3f(1,1,1);
    for (int i = 0; i < 200; i++) glVertex2f(starX[i], starY[i]);
    glEnd();
}

void drawStones() {
   for(int i = 0; i < MAX_STONES; i++){

        if(!stoneActive[i]) continue;

        glPushMatrix();

        glTranslatef(stoneX[i], stoneY[i], 0.0f);
        glRotatef(stoneAngle[i], 0.0f, 0.0f, 1.0f);
        glScalef(stoneScale[i], stoneScale[i], 1.0f);

        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(-0.03f, -0.02f);
        glVertex2f( 0.03f, -0.03f);
        glVertex2f( 0.04f,  0.02f);
        glVertex2f( 0.00f,  0.05f);
        glVertex2f(-0.04f,  0.02f);
        glEnd();

        glPopMatrix();
    }
}

// =============================================================
//  LEVEL 2  –  logic
// =============================================================
void initTinyRockets() {
    for (int i = 0; i < MAX_TINY; i++) {
        tinyActive[i] = true;
        tinyMoveCount[i] = 0;
        if (i < 7) { tinyY[i] = 0.22f; tinyX[i] = -1.0f+i*0.25f; tinyDir[i] = 1; }
        else        { tinyY[i] = 0.10f; tinyX[i] =  1.0f-(i-7)*0.25f; tinyDir[i] = -1; }
    }
}

void initStones() {
    for (int i = 0; i < MAX_STONES; i++) {
        stoneX[i]     = (rand()%200-100)/100.0f;
        stoneY[i]     = (rand()%200-100)/100.0f;
        stoneAngle[i] = rand()%360;
        stoneScale[i] = 0.8f+(rand()%20)/100.0f;
        stoneDir[i]   = (i%2==0) ? 1 : -1;
        stoneActive[i]= true;
    }
}

void resetLevel2() {
    heroX = 0.0f; heroY = -0.75f;
    heroLife = 4; score2 = 0;
    bossX = 0.0f; bossY = 0.75f;
    bossLife = bossMaxLife;
    isFiring = false; isEnemyFiring2 = false;
    heroBulletY[0] = heroBulletY[1] = 2.0f;
    enemyBulletY2 = 2.0f; enemyCooldown = 0;
    gameOver2 = false; gameWin2 = false;
    initTinyRockets();
    initStones();
}

void updateHeroBullets2() {
    if (!isFiring) return;
    bool allOff = true;
    for (int i = 0; i < 2; i++) {
        heroBulletY[i] += 0.05f;
        if (heroBulletY[i] < 1.2f) allOff = false;
    }
    if (allOff) isFiring = false;
}

void updateBossMovement2() {
    bossX = sin(glutGet(GLUT_ELAPSED_TIME) * 0.0014f) * 0.9f;
}

void enemyAutoFire2() {
    if (bossLife <= 0 || gameOver2) { isEnemyFiring2 = false; return; }
    if (enemyCooldown > 0) { enemyCooldown--; return; }
    if (!isEnemyFiring2) {
        isEnemyFiring2 = true;
        enemyBulletX2  = bossX;
        enemyBulletY2  = bossY;
        enemyCooldown  = 50;
    }
}

void updateEnemyBullet2() {
    if (bossLife <= 0 || gameOver2) { isEnemyFiring2 = false; return; }
    if (isEnemyFiring2) {
        enemyBulletY2 -= 0.03f;
        if (enemyBulletY2 < -1.0f) isEnemyFiring2 = false;
    }
}

void updateTinyRocket() {
    for (int i = 0; i < MAX_TINY; i++) {
        if (!tinyActive[i]) continue;
        tinyX[i] += tinyDir[i]*0.0025f;
        if (tinyX[i] >  1.1f) { tinyX[i] =  1.1f; tinyDir[i] = -1; tinyMoveCount[i]++; }
        if (tinyX[i] < -1.1f) { tinyX[i] = -1.1f; tinyDir[i] =  1; tinyMoveCount[i]++; }
        if (tinyMoveCount[i] >= 2) tinyActive[i] = false;
    }
}

void updateStones2() {
    for (int i = 0; i < MAX_STONES; i++) {
        if (!stoneActive[i]) continue;
        stoneAngle[i] += 1.5f*stoneDir[i];
        stoneScale[i] += 0.002f*stoneDir[i];
        if (stoneScale[i] > 1.3f) stoneDir[i] = -1;
        if (stoneScale[i] < 0.7f) stoneDir[i] =  1;
    }
}

void checkHeroDamage2() {
    if (!isEnemyFiring2 || gameOver2) return;
    if (fabs(enemyBulletX2-heroX) < 0.06f &&
        fabs(enemyBulletY2-heroY) < 0.06f) {
        heroLife--;
        isEnemyFiring2 = false;
        enemyBulletY2  = 2.0f;
        if (heroLife <= 0) { heroLife = 0; gameOver2 = true; }
    }
}

void checkBossCollision2() {
    if (bossLife <= 0) return;
    for (int i = 0; i < 2; i++) {
        if (!isFiring) return;
        if (fabs(heroBulletX[i]-bossX) < 0.08f &&
            fabs(heroBulletY[i]-bossY) < 0.08f) {
            bossLife -= 1.5f;
            score2   += 15;
            heroBulletY[i] = 2.0f;
            if (bossLife <= 0) {
                bossLife = 0; isEnemyFiring2 = false;
                enemyBulletY2 = 2.0f; gameWin2 = true;
            }
        }
    }
}

void checkTinyCollision2() {
    for (int i = 0; i < MAX_TINY; i++) {
        if (!tinyActive[i]) continue;
        for (int b = 0; b < 2; b++) {
            if (!isFiring) continue;
            if (fabs(heroBulletX[b]-tinyX[i]) < 0.05f &&
                fabs(heroBulletY[b]-tinyY[i]) < 0.05f) {
                score2 += 10;
                tinyActive[i] = false;
                heroBulletY[b] = 2.0f;
            }
        }
    }
}

void drawHUD2() {
    char buf[50];
    sprintf(buf, "Score: %d", score2);
    glColor3f(1,1,1);
    glRasterPos2f(-0.9f, 0.9f);
    for (int i = 0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
    sprintf(buf, "Life: %d", heroLife);
    glRasterPos2f(-0.9f, 0.83f);
    for (int i = 0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
    if (gameWin2) {
        glColor3f(0,1,0); glRasterPos2f(-0.2f, 0.0f);
        char msg[] = "YOU WIN!";
        for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
    } else if (gameOver2) {
        glColor3f(1,0,0); glRasterPos2f(-0.2f, 0.0f);
        char msg[] = "GAME OVER!";
        for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
    }
}

void level2() {
    background2();
    drawStones();
    drawHUD2();
    level2Rocket(heroX, heroY);
    checkTinyCollision2();

    if (isFiring)
        for (int i = 0; i < 2; i++) drawHeroBullet(heroBulletX[i], heroBulletY[i]);
    if (isEnemyFiring2) drawEnemyBullet2(enemyBulletX2, enemyBulletY2);

    if (bossLife > 0) {
        float ratio = (float)bossLife / bossMaxLife;
        glColor3f(1,0,0);
        glBegin(GL_QUADS);
            glVertex2f(-0.4f,0.95f); glVertex2f(-0.4f+0.8f*ratio,0.95f);
            glVertex2f(-0.4f+0.8f*ratio,0.98f); glVertex2f(-0.4f,0.98f);
        glEnd();
        bossRocketLevel2(bossX, bossY);
    }
    for (int i = 0; i < MAX_TINY; i++)
        if (tinyActive[i]) tinyRocket(tinyX[i], tinyY[i]);
}

// =============================================================
//  LEVEL 3  –  draw helpers
// =============================================================
void l3_drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 50; i++) {
        float a = 2*PI*i/50;
        glVertex2f(x+r*cos(a), y+r*sin(a));
    }
    glEnd();
}

void enemyRocketLevel3(float x, float y) {
    float sx = 1.5f;
    glColor3f(0.05f, 0.08f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y-0.20f);
        glVertex2f(x-0.08f*sx, y+0.12f);
        glVertex2f(x+0.08f*sx, y+0.12f);
    glEnd();
    glColor3f(0.3f, 0.4f, 0.6f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y-0.20f);
        glVertex2f(x-0.08f*sx, y+0.12f);
        glVertex2f(x+0.08f*sx, y+0.12f);
    glEnd();
    glColor3f(0.1f, 0.3f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.08f*sx, y+0.05f); glVertex2f(x-0.25f*sx, y+0.18f); glVertex2f(x-0.08f*sx, y+0.18f);
        glVertex2f(x+0.08f*sx, y+0.05f); glVertex2f(x+0.25f*sx, y+0.18f); glVertex2f(x+0.08f*sx, y+0.18f);
    glEnd();
    glColor3f(0.2f, 0.25f, 0.35f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.14f*sx, y-0.02f); glVertex2f(x-0.10f*sx, y-0.02f);
        glVertex2f(x-0.10f*sx, y+0.10f); glVertex2f(x-0.14f*sx, y+0.10f);
        glVertex2f(x+0.10f*sx, y-0.02f); glVertex2f(x+0.14f*sx, y-0.02f);
        glVertex2f(x+0.14f*sx, y+0.10f); glVertex2f(x+0.10f*sx, y+0.10f);
    glEnd();
    glColor3f(0.4f, 0.7f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y-0.12f); glVertex2f(x-0.035f*sx, y+0.04f); glVertex2f(x+0.035f*sx, y+0.04f);
    glEnd();
    float r = 0.035f;
    glColor3f(0.0f, 1.0f, 1.0f);
    l3_drawCircle(x, y-0.05f, r);
    glColor3f(1.0f, 1.0f, 1.0f);
    l3_drawCircle(x, y-0.05f, 0.015f);
    glColor3f(1.0f, 0.55f, 0.10f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.03f, y+0.12f); glVertex2f(x+0.03f, y+0.12f); glVertex2f(x, y+0.28f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.015f, y+0.12f); glVertex2f(x+0.015f, y+0.12f); glVertex2f(x, y+0.20f);
    glEnd();
    glColor3f(1.0f, 0.65f, 0.15f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.14f*sx, y+0.10f); glVertex2f(x-0.10f*sx, y+0.10f); glVertex2f(x-0.12f*sx, y+0.22f);
        glVertex2f(x+0.10f*sx, y+0.10f); glVertex2f(x+0.14f*sx, y+0.10f); glVertex2f(x+0.12f*sx, y+0.22f);
    glEnd();
}

void level3Rocket(float x, float y) {
    float sx = 0.75f, sy = 0.75f;
    glColor3f(0.55f, 0.55f, 0.58f);
    glBegin(GL_QUADS);
        glVertex2f(x-0.035f*sx, y-0.12f*sy); glVertex2f(x+0.035f*sx, y-0.12f*sy);
        glVertex2f(x+0.035f*sx, y+0.08f*sy); glVertex2f(x-0.035f*sx, y+0.08f*sy);
    glEnd();
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x-0.035f*sx, y-0.12f*sy); glVertex2f(x+0.035f*sx, y-0.12f*sy);
        glVertex2f(x+0.035f*sx, y+0.08f*sy); glVertex2f(x-0.035f*sx, y+0.08f*sy);
    glEnd();
    glColor3f(0.70f, 0.70f, 0.72f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.035f*sx, y+0.08f*sy);
        glVertex2f(x+0.035f*sx, y+0.08f*sy);
        glVertex2f(x, y+0.18f*sy);
    glEnd();
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x-0.035f*sx, y+0.08f*sy);
        glVertex2f(x+0.035f*sx, y+0.08f*sy);
        glVertex2f(x, y+0.18f*sy);
    glEnd();
    glColor3f(0.40f, 0.40f, 0.42f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.035f*sx, y-0.05f*sy); glVertex2f(x-0.10f*sx, y-0.14f*sy); glVertex2f(x-0.035f*sx, y-0.14f*sy);
        glVertex2f(x+0.035f*sx, y-0.05f*sy); glVertex2f(x+0.10f*sx, y-0.14f*sy); glVertex2f(x+0.035f*sx, y-0.14f*sy);
    glEnd();
    glColor3f(0.15f, 0.15f, 0.18f);
    float r = 0.010f;
    for (int j = 0; j < 3; j++) {
        float wy = y + j*0.035f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, wy);
        for (int i = 0; i <= 20; i++) {
            float angle = 2*PI*i/20;
            glVertex2f(x+r*cos(angle), wy+r*sin(angle));
        }
        glEnd();
    }
    glColor3f(1.0f, 0.45f, 0.10f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x-0.02f, y-0.12f*sy);
        glVertex2f(x+0.02f, y-0.12f*sy);
        glVertex2f(x, y-0.22f*sy);
    glEnd();
}

void l3_initStars() {
    for (int i = 0; i < L3_STAR_COUNT; i++) {
        l3_starX[i] = (float(rand())/RAND_MAX)*2 - 1;
        l3_starY[i] = (float(rand())/RAND_MAX)*2 - 1;
    }
}

void l3_drawStars() {
    glColor3f(1,1,1);
    glBegin(GL_POINTS);
    for (int i = 0; i < L3_STAR_COUNT; i++) glVertex2f(l3_starX[i], l3_starY[i]);
    glEnd();
}

void l3_spawnPowerUp(float x, float y) {
    for (int i = 0; i < L3_MAX_POWERUPS; i++) {
        if (!l3_powerActive[i]) {
            l3_powerActive[i] = 1;
            l3_powerX[i] = x; 
            l3_powerY[i] = y;
            l3_powerType[i] = 0;
            break;
        }
    }
}
void l3_randomPowerSpawn()
{
    static int timer = 0;
    timer++;

    if (timer % 60 == 0) // every ~2 seconds
    {
        for (int i = 0; i < L3_MAX_POWERUPS; i++)
        {
            if (!l3_powerActive[i])
            {
                l3_powerActive[i] = 1;

                l3_powerX[i] = ((float)rand() / RAND_MAX) * 1.6f - 0.8f;
                l3_powerY[i] -= 0.01f;

                l3_powerType[i] = rand() % 2;

                printf("POWER SPAWNED TYPE=%d\n", l3_powerType[i]);

                break;
            }
        }
    }
}

void l3_updatePlayerBullets() {
    for (int i = 0; i < L3_MAX_PL_BULLETS; i++) {
        if (l3_pBulletActive[i]) {
            l3_pBulletY[i] += 0.03f;
            if (l3_pBulletY[i] > 1.0f) l3_pBulletActive[i] = 0;
        }
    }
}

void l3_updateEnemyBullets() {
    for (int i = 0; i < L3_MAX_EN_BULLETS; i++) {
        if (l3_eBulletActive[i]) {
            l3_eBulletY[i] -= 0.04f;
            if (l3_eBulletY[i] < -1.0f) l3_eBulletActive[i] = 0;
        }
    }
}

void l3_checkHitEnemy() {
    for (int i = 0; i < L3_MAX_PL_BULLETS; i++) {
        if (l3_pBulletActive[i]) {
            if (fabs(l3_pBulletX[i]-l3_enemyX) < 0.15f &&
                fabs(l3_pBulletY[i]-l3_enemyY) < 0.15f) {
                l3_pBulletActive[i] = 0;
                l3_enemyLife--;
                if (rand()%3 == 0) l3_spawnPowerUp(l3_enemyX, l3_enemyY);
            }
        }
    }
}

void l3_checkHitPlayer() {
    for (int i = 0; i < L3_MAX_EN_BULLETS; i++) {
        if (l3_eBulletActive[i]) {
            if (fabs(l3_eBulletX[i]-l3_normalX) < 0.15f &&
                fabs(l3_eBulletY[i]-l3_normalY) < 0.15f) {
                l3_eBulletActive[i] = 0;
                if (!l3_shieldActive) l3_playerLife--;
            }
        }
    }
}

void l3_checkPowerUpCollision() {
    for (int i = 0; i < L3_MAX_POWERUPS; i++) {
        if (l3_powerActive[i]) {
            if (fabs(l3_powerX[i]-l3_normalX) < 0.08f &&
                fabs(l3_powerY[i]-l3_normalY) < 0.08f) {
                l3_powerY[i] = 1.0f;
                l3_powerX[i] = (float(rand())/RAND_MAX)*1.6f - 0.8f;
                l3_playerLife++;
            }
        }
    }
}

void resetLevel3() {
    l3_normalX = 0.0f; l3_normalY = -0.75f;
    l3_enemyX  = 0.0f; l3_enemyY  =  0.75f;
    l3_enemyDX = 0.01f; l3_timeVar = 0.0f;
    l3_playerLife  = 20; l3_enemyLife = 40;
    l3_shieldActive = 0; l3_shieldTimer = 0;
    l3_gameState = 0;
    for (int i = 0; i < L3_MAX_PL_BULLETS; i++) l3_pBulletActive[i] = 0;
    for (int i = 0; i < L3_MAX_EN_BULLETS; i++) l3_eBulletActive[i] = 0;
    for (int i = 0; i < L3_MAX_POWERUPS; i++) {
        l3_powerActive[i] = 1;
        l3_powerX[i] = (float(rand())/RAND_MAX)*1.6f - 0.8f;
        l3_powerY[i] = (float(rand())/RAND_MAX)*2.0f - 1.0f;
        l3_powerType[i] = 0;
        l3_powerAngle[i] = 0.0f;
    }
}

void level3Display() {
    // Dynamic background based on state
    glBegin(GL_QUADS);
    if (l3_shieldActive) {
        glColor3f(0.00f,0.30f,0.35f); glVertex2f(-1, 1);
        glColor3f(0.00f,0.50f,0.60f); glVertex2f( 1, 1);
        glColor3f(0.00f,0.10f,0.20f); glVertex2f( 1,-1);
        glColor3f(0.00f,0.20f,0.30f); glVertex2f(-1,-1);
    } else if (l3_enemyLife <= 20) {
        glColor3f(0.10f,0.00f,0.25f); glVertex2f(-1, 1);
        glColor3f(0.00f,0.25f,0.40f); glVertex2f( 1, 1);
        glColor3f(0.00f,0.00f,0.10f); glVertex2f( 1,-1);
        glColor3f(0.15f,0.00f,0.30f); glVertex2f(-1,-1);
    } else if (l3_enemyLife > 27) {
        glColor3f(0.02f,0.02f,0.08f);
        glVertex2f(-1,1); glVertex2f(1,1); glVertex2f(1,-1); glVertex2f(-1,-1);
    } else {
        glColor3f(0.05f,0.05f,0.15f);
        glVertex2f(-1,1); glVertex2f(1,1); glVertex2f(1,-1); glVertex2f(-1,-1);
    }
    glEnd();

    l3_drawStars();
    enemyRocketLevel3(l3_enemyX, l3_enemyY);
    level3Rocket(l3_normalX, l3_normalY);

    // Shield ring
    if (l3_shieldActive) {
        float r = 0.18f;
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 50; i++) {
            float a = 2*PI*i/50;
            glVertex2f(l3_normalX+r*cos(a), l3_normalY+r*sin(a));
        }
        glEnd();
    }

    // Player bullets
    glColor3f(0.0f, 1.0f, 0.4f);
    for (int i = 0; i < L3_MAX_PL_BULLETS; i++) {
        if (l3_pBulletActive[i]) {
            glBegin(GL_QUADS);
                glVertex2f(l3_pBulletX[i]-0.012f, l3_pBulletY[i]-0.02f);
                glVertex2f(l3_pBulletX[i]+0.012f, l3_pBulletY[i]-0.02f);
                glVertex2f(l3_pBulletX[i]+0.012f, l3_pBulletY[i]+0.02f);
                glVertex2f(l3_pBulletX[i]-0.012f, l3_pBulletY[i]+0.02f);
            glEnd();
        }
    }
    // Enemy bullets
    glColor3f(1, 0, 0);
    for (int i = 0; i < L3_MAX_EN_BULLETS; i++) {
        if (l3_eBulletActive[i]) {
            glBegin(GL_QUADS);
                glVertex2f(l3_eBulletX[i]-0.018f, l3_eBulletY[i]-0.02f);
                glVertex2f(l3_eBulletX[i]+0.018f, l3_eBulletY[i]-0.02f);
                glVertex2f(l3_eBulletX[i]+0.018f, l3_eBulletY[i]+0.02f);
                glVertex2f(l3_eBulletX[i]-0.018f, l3_eBulletY[i]+0.02f);
            glEnd();
        }
    }
    // Power-ups
    for (int i = 0; i < L3_MAX_POWERUPS; i++) {
        if (l3_powerActive[i] && l3_powerType[i] == 0) {
            glPushMatrix();
            glTranslatef(l3_powerX[i], l3_powerY[i], 0);
            glScalef(1.0f, 0.4f, 1.0f);
            //glRotatef(l3_powerAngle[i], 0,0,1);
            glColor3f(0.0f,1.0f,1.0f);
            float r = 0.04f;
            glBegin(GL_POLYGON);
            for (int j = 0; j < 6; j++) {
                float a = 2*PI*j/6;
                glVertex2f(r*cos(a), r*sin(a));
            }
            glEnd();
            glColor3f(1,1,1);
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < 6; j++) {
                float a = 2*PI*j/6;
                glVertex2f(r*cos(a), r*sin(a));
            }
            glEnd();
            glPopMatrix();
        }
    }
    // HUD
    glColor3f(1,0,0); glRasterPos2f(-0.95f,0.9f);
    char etxt[50]; sprintf(etxt, "Enemy Life: %d", l3_enemyLife);
    for (int i = 0; etxt[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, etxt[i]);

    glColor3f(0,1,0); glRasterPos2f(0.5f,0.9f);
    char ptxt[50]; sprintf(ptxt, "Player Life: %d", l3_playerLife);
    for (int i = 0; ptxt[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ptxt[i]);

    glColor3f(1,1,1); glRasterPos2f(-0.95f,0.85f);
    char htxt[] = "Press S: Shield";
    for (int i = 0; htxt[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, htxt[i]);

    if (l3_gameState == 1) {
        glColor3f(0,1,0); glRasterPos2f(-0.2f,0.0f);
        char msg[] = "YOU WIN!";
        for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
    }
    if (l3_gameState == 2) {
        glColor3f(1,0,0); glRasterPos2f(-0.2f,0.0f);
        char msg[] = "GAME OVER!";
        for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
    }
}

// =============================================================
//  LEVEL 4  –  helpers
// =============================================================
void l4_drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 100; i++) {
        float a = 2*PI*i/100;
        glVertex2f(x+cos(a)*r, y+sin(a)*r);
    }
    glEnd();
}

void l4_initStars() {
    for (int i = 0; i < 100; i++) {
        l4_starX[i] = ((rand()%200)-100)/100.0f;
        l4_starY[i] = ((rand()%200)-100)/100.0f;
    }
}

void l4_initAsteroids() {
    for (int i = 0; i < L4_MAX_ASTEROIDS; i++) {
        l4_asteroidActive[i] = true;
        l4_asteroidX[i] = (rand()%200-100)/100.0f;
        l4_asteroidY[i] = 1.0f+(rand()%100)/100.0f;
        l4_asteroidR[i] = 0.05f+(rand()%10)/100.0f;
    }
}

void resetLevel4() {
    l4_normalX = 0.0f; l4_normalY = -0.75f;
    l4_enemyX  = 0.0f; l4_enemyY  =  0.80f;
    l4_enemyDX = 0.01f; l4_enemyDY = 0.005f;
    l4_enemyLife = 15; l4_playerLife = 3;
    l4_gameOver = false; l4_win = false;
    l4_enemyShootDelay = 0; l4_playerShootCounter = 0;
    for (int i = 0; i < L4_MAX_BULLETS; i++) {
        l4_bulletActive[i] = false;
        l4_pBulletActive[i] = false;
    }
    l4_initAsteroids();
}

void l4_drawCloud(float x, float y) {
    glColor3f(1,1,1);
    l4_drawCircle(x,       y,       0.05f);
    l4_drawCircle(x+0.05f, y+0.02f, 0.06f);
    l4_drawCircle(x-0.05f, y+0.02f, 0.05f);
}

void l4_drawDayBackground() {
    glBegin(GL_QUADS);
        glColor3f(0.4f,0.7f,1.0f);  glVertex2f(-1,-1);
        glColor3f(0.6f,0.85f,1.0f); glVertex2f( 1,-1);
        glColor3f(0.2f,0.6f,1.0f);  glVertex2f( 1, 1);
        glColor3f(0.4f,0.8f,1.0f);  glVertex2f(-1, 1);
    glEnd();
    glColor3f(1,1,0);
    l4_drawCircle(0.7f, 0.75f, 0.12f);
    for (int i = 0; i < 5; i++) {
        l4_drawCloud(l4_cloudX[i], l4_cloudY[i]);
        l4_cloudX[i] += 0.001f;
        if (l4_cloudX[i] > 1.2f) l4_cloudX[i] = -1.2f;
    }
}

void l4_drawNightBackground() {
    glClearColor(0,0.1f,0.15f,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
        glColor3f(1,1,1);
        glVertex2f(l4_starX[i], l4_starY[i]);
        l4_starY[i] -= 0.01f;
        if (l4_starY[i] < -1.0f) l4_starY[i] = 1.0f;
    }
    glEnd();
    glColor3f(0.2f,0.9f,0.2f);
    l4_drawCircle(-0.75f, 0.75f, 0.15f);
    l4_drawCircle( 0.75f,-0.50f, 0.18f);
}

void l4_background() {
    if (l4_dayMode) l4_drawDayBackground();
    else            l4_drawNightBackground();
}

void l4_drawNormalRocket() {
    glPushMatrix();
    glTranslatef(l4_normalX, l4_normalY, 0);
    glScalef(0.35f, 0.35f, 1.0f);
    glColor3f(0.55f,0.0f,0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,1.2f); glVertex2f(-0.35f,0); glVertex2f(0.35f,0);
    glEnd();
    glColor3f(0.9f,0.2f,0.4f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,1.0f); glVertex2f(-0.20f,0); glVertex2f(0.20f,0);
    glEnd();
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glVertex2f(-0.45f,0.1f); glVertex2f(-0.30f,0.1f); glVertex2f(-0.30f,0.8f); glVertex2f(-0.45f,0.8f);
        glVertex2f( 0.30f,0.1f); glVertex2f( 0.45f,0.1f); glVertex2f( 0.45f,0.8f); glVertex2f( 0.30f,0.8f);
    glEnd();
    glColor3f(0.7f,0.1f,0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.35f,0.2f); glVertex2f(-0.65f,0); glVertex2f(-0.35f,0.6f);
        glVertex2f( 0.35f,0.2f); glVertex2f( 0.65f,0); glVertex2f( 0.35f,0.6f);
    glEnd();
    glColor3f(0.6f,1.0f,1.0f);
    l4_drawCircle(0,0.5f,0.10f);
    glColor3f(1.0f,0.2f,0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.12f,0); glVertex2f(0.12f,0); glVertex2f(0,-0.45f);
    glEnd();
    glPopMatrix();
}

void l4_drawEnemyRocket() {
    glPushMatrix();
    glTranslatef(l4_enemyX, l4_enemyY, 0);
    glScalef(0.42f,-0.42f,1.0f);
    glColor3f(0,0.9f,1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,1.3f); glVertex2f(-0.40f,0); glVertex2f(0.40f,0);
    glEnd();
    glColor3f(0,0.6f,1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,1.1f); glVertex2f(-0.25f,0); glVertex2f(0.25f,0);
    glEnd();
    glColor3f(0.8f,0.8f,0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-0.55f,0.1f); glVertex2f(-0.35f,0.1f); glVertex2f(-0.35f,0.9f); glVertex2f(-0.55f,0.9f);
        glVertex2f( 0.35f,0.1f); glVertex2f( 0.55f,0.1f); glVertex2f( 0.55f,0.9f); glVertex2f( 0.35f,0.9f);
    glEnd();
    glColor3f(0,0.5f,1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.40f,0.25f); glVertex2f(-0.80f,0); glVertex2f(-0.40f,0.7f);
        glVertex2f( 0.40f,0.25f); glVertex2f( 0.80f,0); glVertex2f( 0.40f,0.7f);
    glEnd();
    glColor3f(0.6f,1,1);
    l4_drawCircle(0,0.6f,0.12f);
    glColor3f(0,1,1);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.15f,0); glVertex2f(0.15f,0); glVertex2f(0,-0.50f);
    glEnd();
    glPopMatrix();
}

void l4_drawBullets() {
    glColor3f(1,0.3f,0.3f);
    for (int i = 0; i < L4_MAX_BULLETS; i++)
        if (l4_bulletActive[i]) l4_drawCircle(l4_bulletX[i], l4_bulletY[i], 0.02f);
}

void l4_drawPlayerBullets() {
    glColor3f(0.3f,1,0.3f);
    for (int i = 0; i < L4_MAX_BULLETS; i++)
        if (l4_pBulletActive[i]) l4_drawCircle(l4_pBulletX[i], l4_pBulletY[i], 0.015f);
}

void l4_drawAsteroids() {
    glColor3f(0.5f,0.5f,0.5f);
    for (int i = 0; i < L4_MAX_ASTEROIDS; i++) {
        if (l4_asteroidActive[i]) {
            float x = l4_asteroidX[i], y = l4_asteroidY[i];
            float r = l4_asteroidR[i]*0.5f;
            glBegin(GL_POLYGON);
            for (int j = 0; j < 10; j++) {
                float a = 2*PI*j/10;
                float off = (rand()%30)/100.0f;
                float radius = r + off*r;
                glVertex2f(x+cos(a)*radius, y+sin(a)*radius);
            }
            glEnd();
        }
    }
}

void l4_drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void l4_fireEnemyBullet() {
    for (int i = 0; i < L4_MAX_BULLETS; i++) {
        if (!l4_bulletActive[i]) {
            l4_bulletActive[i] = true;
            l4_bulletX[i] = l4_enemyX;
            l4_bulletY[i] = l4_enemyY;
            break;
        }
    }
}

void l4_fireTripleBullet() {
    for (int i = 0; i < L4_MAX_BULLETS; i++) {
        if (!l4_pBulletActive[i]) {
            l4_pBulletActive[i] = true;
            l4_pBulletX[i] = l4_normalX;
            l4_pBulletY[i] = l4_normalY+0.2f;
            if (i+1 < L4_MAX_BULLETS) {
                l4_pBulletActive[i+1] = true;
                l4_pBulletX[i+1] = l4_normalX-0.08f;
                l4_pBulletY[i+1] = l4_normalY+0.2f;
            }
            if (i+2 < L4_MAX_BULLETS) {
                l4_pBulletActive[i+2] = true;
                l4_pBulletX[i+2] = l4_normalX+0.08f;
                l4_pBulletY[i+2] = l4_normalY+0.2f;
            }
            break;
        }
    }
}

void level4Display() {
    l4_background();
    l4_drawAsteroids();
    l4_drawEnemyRocket();
    l4_drawNormalRocket();
    if (!l4_gameOver) {
        l4_drawBullets();
        l4_drawPlayerBullets();
    }
    char ltxt[20]; sprintf(ltxt,"Life: %d",l4_playerLife);
    glColor3f(1,1,1); l4_drawText(-0.95f,0.90f,ltxt);
    char etxt[20]; sprintf(etxt,"Enemy: %d",l4_enemyLife);
    glColor3f(1,0.5f,0.5f); l4_drawText(0.60f,0.90f,etxt);
    if (l4_gameOver) {
        if (l4_enemyLife <= 0) {
            glColor3f(0,1,0); l4_drawText(-0.15f,0.0f,"YOU ARE ROCK BUDDY, YOU WIN!");
        } else {
            glColor3f(1,0,0); l4_drawText(-0.15f,0.0f,"GAME OVER");
        }
    }
}

// =============================================================
//  MENU / GAME OVER screens
// =============================================================
void drawMenu() {
    glColor3f(0.02f,0.02f,0.08f);
    glBegin(GL_QUADS);
        glVertex2f(-1,1); glVertex2f(-1,-1); glVertex2f(1,-1); glVertex2f(1,1);
    glEnd();
    glBegin(GL_POINTS); glColor3f(1,1,1);
    for (int i = 0; i < 200; i++) glVertex2f(starX[i], starY[i]);
    glEnd();

    glColor3f(1,1,1); glRasterPos2f(-0.35f,0.7f);
    char title[] = "Cosmic Fighter";
    for (int i = 0; title[i]; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[i]);

    // Level buttons
    const char* labels[] = { "Press [1] for Level 1", "Press [2] for Level 2",
                              "Press [3] for Level 3", "Press [4] for Level 4" };
    float bY[] = { 0.45f, 0.25f, 0.05f, -0.15f };
    for (int k = 0; k < 4; k++) {
        glBegin(GL_LINE_LOOP);
            glVertex2f(-0.38f, bY[k]+0.10f); glVertex2f(0.38f, bY[k]+0.10f);
            glVertex2f(0.38f, bY[k]+0.02f);  glVertex2f(-0.38f, bY[k]+0.02f);
        glEnd();
        glRasterPos2f(-0.33f, bY[k]+0.04f);
        for (int i = 0; labels[k][i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, labels[k][i]);
    }

    glColor3f(0.8f,0.8f,0.8f); glRasterPos2f(-0.38f,-0.75f);
    char ctrl[] = "Fire: SPACE | Move: Left/Right Arrow";
    for (int i = 0; ctrl[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ctrl[i]);
    glRasterPos2f(-0.28f,-0.83f);
    char back[] = "Press [b] to return to menu";
    for (int i = 0; back[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, back[i]);
}

void drawGameOver() {
    glColor3f(1,0,0); glRasterPos2f(-0.25f,0.2f);
    char msg[] = "GAME OVER";
    for (int i = 0; msg[i]; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
    glColor3f(1,1,1); glRasterPos2f(-0.45f,0.0f);
    char msg2[] = "Press [1]-[4] for a level  |  [b] for menu";
    for (int i = 0; msg2[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg2[i]);
}

// =============================================================
//  GLUT callbacks
// =============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch (gameState) {
        case STATE_MENU:     drawMenu();        break;
        case STATE_LEVEL1:   level1();          break;
        case STATE_LEVEL2:   level2();          break;
        case STATE_LEVEL3:   level3Display();   break;
        case STATE_LEVEL4:   level4Display();   break;
        case STATE_GAMEOVER: drawGameOver();    break;
    }
    glFlush();
}

void specialKeys(int key, int x, int y) {
    if (gameState == STATE_LEVEL1) {
        if (key == GLUT_KEY_LEFT)  rocketX -= speed1;
        if (key == GLUT_KEY_RIGHT) rocketX += speed1;
        if (rocketX >  0.9f) rocketX =  0.9f;
        if (rocketX < -0.9f) rocketX = -0.9f;
    }
    else if (gameState == STATE_LEVEL2) {
        if (key == GLUT_KEY_LEFT)  heroX -= heroSpeed;
        if (key == GLUT_KEY_RIGHT) heroX += heroSpeed;
        if (heroX >  0.8f) heroX =  0.8f;
        if (heroX < -0.8f) heroX = -0.8f;
    }
    else if (gameState == STATE_LEVEL3) {
        float step = 0.08f;
        if (key == GLUT_KEY_LEFT)  { l3_normalX -= step; if(l3_normalX<-0.8f) l3_normalX=-0.8f; }
        if (key == GLUT_KEY_RIGHT) { l3_normalX += step; if(l3_normalX> 0.8f) l3_normalX= 0.8f; }
        if (key == GLUT_KEY_UP) {
            for (int j = 0; j < L3_MAX_PL_BULLETS; j++) {
                if (!l3_pBulletActive[j]) {
                    l3_pBulletActive[j] = 1;
                    l3_pBulletX[j] = l3_normalX;
                    l3_pBulletY[j] = l3_normalY+0.1f;
                    break;
                }
            }
        }
    }
    else if (gameState == STATE_LEVEL4) {
        float step = 0.05f;
        if (key == GLUT_KEY_LEFT)  { l4_normalX -= step; if(l4_normalX<-0.75f) l4_normalX=-0.75f; }
        if (key == GLUT_KEY_RIGHT) { l4_normalX += step; if(l4_normalX> 0.75f) l4_normalX= 0.75f; }
        if (key == GLUT_KEY_UP)    { l4_normalY += step; if(l4_normalY> 0.80f) l4_normalY= 0.80f; }
        if (key == GLUT_KEY_DOWN)  { l4_normalY -= step; if(l4_normalY<-0.80f) l4_normalY=-0.80f; }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    // Level selection / menu navigation (available from anywhere)
    if (key == '1') { gameState = STATE_LEVEL1; resetLevel1(); return; }
    if (key == '2') { gameState = STATE_LEVEL2; resetLevel2(); return; }
    if (key == '3') { gameState = STATE_LEVEL3; resetLevel3(); return; }
    if (key == '4') { gameState = STATE_LEVEL4; resetLevel4(); return; }
    if (key == 'b' || key == 'B') { gameState = STATE_MENU; return; }

    // Level 1 controls
    if (gameState == STATE_LEVEL1 && key == ' ' && bulletActive1 == 0) {
        bulletActive1 = 1;
        bulletX1 = rocketX;
        bulletY1 = rocketY+0.12f;
    }

    // Level 2 controls
    if (gameState == STATE_LEVEL2 && key == ' ' && !isFiring) {
        isFiring = true;
        heroBulletX[0] = heroX-0.02f;
        heroBulletX[1] = heroX+0.02f;
        heroBulletY[0] = heroY;
        heroBulletY[1] = heroY;
    }

    // Level 3 controls
    if (gameState == STATE_LEVEL3) {
        if (key == ' ') {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < L3_MAX_PL_BULLETS; j++) {
                    if (!l3_pBulletActive[j]) {
                        l3_pBulletActive[j] = 1;
                        l3_pBulletX[j] = l3_normalX+(i-1.5f)*0.05f;
                        l3_pBulletY[j] = l3_normalY+0.1f;
                        break;
                    }
                }
            }
        }
        if (key == 's' || key == 'S') { l3_shieldActive = 1; l3_shieldTimer = 150; }
    }

    // Level 4 controls
    if (gameState == STATE_LEVEL4) {
        if (key == ' ')          l4_fireTripleBullet();
        if (key=='t'||key=='T') l4_dayMode = !l4_dayMode;
        if (key=='r'||key=='R') resetLevel4();
    }
}

void update(int value) {
    // Shared star scroll for Menu, L1 & L2
    if (gameState == STATE_MENU || gameState == STATE_LEVEL1 || gameState == STATE_LEVEL2) updateStars();

    // ── Level 1 update ──────────────────────────────────────
    if (gameState == STATE_LEVEL1) {
        moonX += 0.002f;
        if (moonX > 1.2f) moonX = -1.2f;
        moonScale -= scaleSpeed;
        if (moonScale < 0.3f) moonScale = 1.0f;
        moonAngle += 1.0f;
        if (moonAngle > 360) moonAngle = 0;

        if (!levelTriggered && asteroidsDestroyed >= 10) {
            enemyActive1 = 1; levelTriggered = 1;
        }
        updateBullet1();
        updateAsteroids1();
        checkAsteroidCollision1();
        checkRocketAsteroidCollision1();
        checkEnemyCollision1();
        enemyShoot1();
        updateEnemyBullets1();
        updateEnemy1();
    }

    // ── Level 2 update ──────────────────────────────────────
    if (gameState == STATE_LEVEL2 && !gameOver2) {
        updateHeroBullets2();
        enemyAutoFire2();
        updateEnemyBullet2();
        updateBossMovement2();
        updateTinyRocket();
        checkHeroDamage2();
        checkTinyCollision2();
        checkBossCollision2();
    }
    if (gameState == STATE_LEVEL2) updateStones2();

    // ── Level 3 update ──────────────────────────────────────
    if (gameState == STATE_LEVEL3 && l3_gameState == 0) {
        l3_enemyX += l3_enemyDX;
        l3_timeVar += 0.03f;
        l3_enemyY = 0.65f + 0.1f*sin(l3_timeVar);
        if (l3_enemyX > 0.85f || l3_enemyX < -0.85f) l3_enemyDX = -l3_enemyDX;

        // Star scroll
        for (int i = 0; i < L3_STAR_COUNT; i++) {
            l3_starY[i] -= 0.003f;
            if (l3_starY[i] < -1.0f) {
                l3_starY[i] = 1.0f;
                l3_starX[i] = (float(rand())/RAND_MAX)*2-1;
            }
        }

        // Enemy shoot
        static int l3_shootTimer = 0;
        l3_shootTimer++;
        if (l3_shootTimer > 40) {
            l3_shootTimer = 0;
            for (int i = 0; i < L3_MAX_EN_BULLETS; i++) {
                if (!l3_eBulletActive[i]) {
                    l3_eBulletActive[i] = 1;
                    l3_eBulletX[i] = l3_enemyX;
                    l3_eBulletY[i] = l3_enemyY;
                    break;
                }
            }
        }

        l3_updatePlayerBullets();
        l3_updateEnemyBullets();
        l3_checkHitEnemy();
        l3_checkHitPlayer();

        if (l3_enemyLife <= 0) l3_gameState = 1;
        if (l3_playerLife <= 0) l3_gameState = 2;

        for (int i = 0; i < L3_MAX_POWERUPS; i++) {
            if (l3_powerActive[i]) {
                l3_powerY[i]    -= 0.005f; // POWERUP FALL SPEED
                l3_powerAngle[i] += 5.0f; // POWERUP ROTATION
                if (l3_powerY[i] < -1.0f) {
                    l3_powerY[i] = 1.0f;
                    l3_powerX[i] = (float(rand())/RAND_MAX)*1.6f - 0.8f;
                    l3_powerType[i] = rand()%2;
                }
            }
        }

        if (l3_shieldActive) {
            l3_shieldTimer--;
            if (l3_shieldTimer <= 0) l3_shieldActive = 0;
        }
        l3_randomPowerSpawn();
        l3_checkPowerUpCollision();

    }

    // ── Level 4 update ──────────────────────────────────────
    if (gameState == STATE_LEVEL4) {
        if (!l4_gameOver) {
            // Auto-shoot
            l4_playerShootCounter++;
            if (l4_playerShootCounter >= 15) {
                // fire single bullet (not triple auto) for auto-fire
                for (int i = 0; i < L4_MAX_BULLETS; i++) {
                    if (!l4_pBulletActive[i]) {
                        l4_pBulletActive[i] = true;
                        l4_pBulletX[i] = l4_normalX;
                        l4_pBulletY[i] = l4_normalY+0.2f;
                        break;
                    }
                }
                l4_playerShootCounter = 0;
            }

            // Move asteroids
            for (int i = 0; i < L4_MAX_ASTEROIDS; i++) {
                if (l4_asteroidActive[i]) {
                    l4_asteroidY[i] -= 0.01f;
                    if (l4_asteroidY[i] < -1.2f) {
                        l4_asteroidY[i] = 1.0f;
                        l4_asteroidX[i] = (rand()%200-100)/100.0f;
                    }
                    // Asteroid hits player
                    float dx = l4_asteroidX[i]-l4_normalX;
                    float dy = l4_asteroidY[i]-l4_normalY;
                    if (sqrt(dx*dx+dy*dy) < l4_asteroidR[i]+0.10f) {
                        l4_asteroidY[i] = 1.0f;
                        l4_asteroidX[i] = (rand()%200-100)/100.0f;
                        l4_playerLife--;
                        if (l4_playerLife <= 0) l4_gameOver = true;
                    }
                }
            }

            // Bullet vs asteroid
            for (int i = 0; i < L4_MAX_BULLETS; i++) {
                if (l4_pBulletActive[i]) {
                    for (int j = 0; j < L4_MAX_ASTEROIDS; j++) {
                        if (l4_asteroidActive[j]) {
                            float dx = l4_pBulletX[i]-l4_asteroidX[j];
                            float dy = l4_pBulletY[i]-l4_asteroidY[j];
                            if (sqrt(dx*dx+dy*dy) < l4_asteroidR[j]) {
                                l4_asteroidY[j] = 1.0f;
                                l4_asteroidX[j] = (rand()%200-100)/100.0f;
                                l4_pBulletActive[i] = false;
                                break;
                            }
                        }
                    }
                }
            }

            // Enemy movement
            l4_enemyX += l4_enemyDX; l4_enemyY += l4_enemyDY;
            if (l4_enemyX > 0.6f || l4_enemyX < -0.6f) l4_enemyDX = -l4_enemyDX;
            if (l4_enemyY > 0.9f || l4_enemyY < 0.2f)  l4_enemyDY = -l4_enemyDY;

            // Enemy shoot
            l4_enemyShootDelay--;
            if (l4_enemyShootDelay <= 0) {
                l4_fireEnemyBullet();
                l4_enemyShootDelay = (rand()%20)+5;
            }

            // Move enemy bullets
            for (int i = 0; i < L4_MAX_BULLETS; i++) {
                if (l4_bulletActive[i]) {
                    l4_bulletY[i] -= 0.02f;
                    if (l4_bulletY[i] < -1.0f) l4_bulletActive[i] = false;
                }
            }

            // Player bullet hits enemy
            for (int i = 0; i < L4_MAX_BULLETS; i++) {
                if (l4_pBulletActive[i]) {
                    float dx = l4_pBulletX[i]-l4_enemyX;
                    float dy = l4_pBulletY[i]-l4_enemyY;
                    if (sqrt(dx*dx+dy*dy) < 0.15f) {
                        l4_pBulletActive[i] = false;
                        l4_enemyLife--;
                        if (l4_enemyLife <= 0) { l4_win = true; l4_gameOver = true; }
                    }
                }
            }

            // Move player bullets
            for (int i = 0; i < L4_MAX_BULLETS; i++) {
                if (l4_pBulletActive[i]) {
                    l4_pBulletY[i] += 0.03f;
                    if (l4_pBulletY[i] > 1.0f) l4_pBulletActive[i] = false;
                }
            }

            // Enemy bullet hits player
            for (int i = 0; i < L4_MAX_BULLETS; i++) {
                if (l4_bulletActive[i]) {
                    float dx = l4_bulletX[i]-l4_normalX;
                    float dy = l4_bulletY[i]-l4_normalY;
                    if (sqrt(dx*dx+dy*dy) < 0.12f) {
                        l4_bulletActive[i] = false;
                        l4_playerLife--;
                        if (l4_playerLife <= 0) l4_gameOver = true;
                    }
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// =============================================================
//  main
// =============================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 900);
    glutCreateWindow("Cosmic Fighter – All Levels");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glPointSize(2.0f);
    glClearColor(0,0,0,0);

    // Init shared star field
    for (int i = 0; i < 200; i++) {
        starX[i] = (rand()%200-100)/100.0f;
        starY[i] = (rand()%200-100)/100.0f;
    }

    // Init level-specific star fields
    l3_initStars();
    l4_initStars();

    // Pre-init Level 1 asteroids
    l1_initAsteroids();

    // Pre-init Level 4 state
    for (int i = 0; i < L4_MAX_BULLETS; i++) {
        l4_bulletActive[i]  = false;
        l4_pBulletActive[i] = false;
    }
    l4_initAsteroids();

    // Pre-init Level 3 power-ups
    for (int i = 0; i < L3_MAX_POWERUPS; i++) {
        l3_powerActive[i] = 1;
        l3_powerX[i] = (float(rand())/RAND_MAX)*1.6f - 0.8f;
        l3_powerY[i] = (float(rand())/RAND_MAX)*2.0f - 1.0f;
        l3_powerType[i] = 0;
        l3_powerAngle[i] = 0.0f;
    }

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
