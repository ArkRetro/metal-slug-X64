/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06a138.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6a138(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  unaff_A6[0x14] = 0xe1cb2;
  candidate_4c1c();
  *unaff_A6 = 0x6a156;
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1aca2();
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x7fff;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_4c24();
  return;
}
