/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0369ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_369ac(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_1b1dc();
  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  *unaff_A6 = 0x369e2;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    sVar3 = candidate_49e8();
    uVar4 = 0;
    if (sVar3 == 0) {
      candidate_427a();
      candidate_1945c();
      if (!(bool)uVar4) {
        return;
      }
      candidate_4192();
      goto candidate_4188;
    }
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  *unaff_A6 = 0x36a3a;
  uVar4 = 0;
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    candidate_49e8();
  }
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar4) {
    return;
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
