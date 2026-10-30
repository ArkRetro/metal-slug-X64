/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05a202.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5a202(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  }
  candidate_1436a();
  sVar3 = candidate_49e8();
  uVar4 = 0;
  if ((sVar3 != 0) && (uVar4 = 0, *(short *)((int)unaff_A6 + 0x6e) == 0)) {
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 5;
    candidate_40bc();
    uVar4 = 0;
    candidate_1acf2();
  }
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar4) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
