/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0454fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_454fa(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined1 *)(unaff_A5 + 0x400b) = 1;
  *(undefined4 *)(unaff_A5 + 0x695c) = 0x20000;
  candidate_c7b48();
  *unaff_A6 = 0x45518;
  if (*(char *)(unaff_A6 + 0x24) == '\0') {
    sVar3 = *(short *)(unaff_A6 + 0x23);
    bVar4 = sVar3 == 0;
    sVar3 = sVar3 + -1;
    *(short *)(unaff_A6 + 0x23) = sVar3;
    if (sVar3 < 0) goto LAB_00045536;
  }
  else {
    bVar4 = false;
    if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
LAB_00045536:
      candidate_45572();
      if (bVar4) {
        *(undefined1 *)(unaff_A5 + 0x400b) = 0;
        *(undefined1 *)(unaff_A5 + 0x400c) = 0;
        *(undefined4 *)(unaff_A5 + 0x695c) = 0;
        candidate_c7b48();
        *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
        candidate_455ea();
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
  }
  return;
}
