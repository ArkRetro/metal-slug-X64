/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a2e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a2e4(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  if (*(char *)(unaff_A6 + 0x1d) == '\0') {
    return;
  }
  *unaff_A6 = 0x3a2f4;
  sVar3 = *(short *)(unaff_A6 + 0x1c);
  *(short *)(unaff_A6 + 0x1c) = sVar3 + -1;
  if (sVar3 == 0) {
    candidate_1436a();
    *(undefined2 *)(unaff_A6 + 0x1c) = *(undefined2 *)((int)unaff_A6 + 0x6e);
    bVar4 = false;
    candidate_19504();
    if (bVar4) {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    bVar4 = false;
    candidate_40bc();
    if (bVar4) {
      candidate_1b220();
    }
  }
  return;
}
