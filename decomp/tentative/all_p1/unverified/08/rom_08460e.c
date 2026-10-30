/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08460e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8460e(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar5;

  candidate_1ac38();
  candidate_1ad8c();
  candidate_427a();
  candidate_1aeda();
  if ((bool)in_CF) {
    sVar3 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0xf);
    if (sVar3 < 0) {
      sVar3 = -sVar3;
    }
    sVar4 = *(short *)(extraout_A0 + 0x44) - *(short *)(unaff_A6 + 0x11);
    if (sVar4 < 0) {
      sVar4 = -sVar4;
    }
    if (((-((short)(sVar3 - *(short *)(unaff_A6 + 0x21)) < 0) !=
          -((short)(sVar4 - *(short *)(unaff_A6 + 0x21)) < 0)) && (10 < *(short *)(unaff_A6 + 0xf)))
       && (*(short *)(unaff_A6 + 0xf) < 0x136)) {
      *unaff_A6 = candidate_846ce;
    }
  }
  candidate_4c24();
  if (*(char *)((int)unaff_A6 + 0x5a) != '\0') {
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) &&
       (*(char *)((int)unaff_A6 + 0x5a) == '\x06')) {
      candidate_120f8();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_846da;
  }
  bVar5 = false;
  candidate_1945c();
  if (!bVar5) {
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
