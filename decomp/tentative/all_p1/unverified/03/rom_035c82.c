/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035c82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_35c82(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  char cVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  sVar3 = candidate_8c0c2();
  if (-1 < sVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x99) = 10;
    *unaff_A6 = 0x35c9e;
    cVar4 = *(char *)((int)unaff_A6 + 0x99);
    while (cVar4 != '\0') {
      candidate_40bc();
      candidate_1b220();
      cVar4 = candidate_29f2();
      *(char *)(extraout_A0 + 0x9a) = cVar4 + -0x40;
      *(short *)(extraout_A0 + 0x28) = (short)(char)(cVar4 + -0x40) + *(short *)(extraout_A0 + 0x28)
      ;
      cVar4 = *(char *)((int)unaff_A6 + 0x99) + -1;
      *(char *)((int)unaff_A6 + 0x99) = cVar4;
    }
    candidate_49e8();
  }
  if (-0x40 < *(short *)(unaff_A6 + 0xf)) {
    candidate_1abc8();
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
