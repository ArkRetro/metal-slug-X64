/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0834b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_834b0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_1abc8();
  sVar3 = *(short *)((int)unaff_A6 + 0x6e);
  uVar4 = sVar3 == 0;
  *(short *)((int)unaff_A6 + 0x6e) = sVar3 + -1;
  if ((short)(sVar3 + -1) == 0 || sVar3 < 1) {
    candidate_411c();
    uVar4 = 0;
    candidate_1acf2();
    candidate_83674();
  }
  candidate_1945c();
  if (!(bool)uVar4) {
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
