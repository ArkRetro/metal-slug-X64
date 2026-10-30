/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a00a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a00a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_120f8();
  candidate_120f8();
  candidate_4256();
  *unaff_A6 = 0x8a040;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
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
  if (((*(char *)((int)unaff_A6 + 0x87) != '\0') && (-5 < *(short *)(unaff_A6 + 0xf))) &&
     (*(short *)(unaff_A6 + 0xf) < 0x145)) {
    candidate_49e8();
  }
  return;
}
