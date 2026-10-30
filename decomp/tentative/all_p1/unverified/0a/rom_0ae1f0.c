/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae1f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae1f0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  sVar3 = *(short *)((int)unaff_A6 + 0x6e);
  *(short *)((int)unaff_A6 + 0x6e) = sVar3 + -1;
  if (sVar3 == 0) {
    candidate_9bb0();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (*(char *)(unaff_A6 + 0x1c) != '\0') {
    *(char *)((int)unaff_A6 + 0x71) = -(*(char *)((int)unaff_A6 + 0x71) == '\0');
    candidate_9bb0();
  }
  return;
}
