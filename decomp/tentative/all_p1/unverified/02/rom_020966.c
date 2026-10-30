/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020966.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20966(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  sVar3 = *(short *)((int)unaff_A6 + 0x6e) + -1;
  *(short *)((int)unaff_A6 + 0x6e) = sVar3;
  if (sVar3 == 0) {
    candidate_c8ce6();
    candidate_9c0a();
    *(undefined1 *)(extraout_A0 + 0x60) = 0xff;
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
