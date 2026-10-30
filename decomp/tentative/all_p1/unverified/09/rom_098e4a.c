/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x098e4a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_98e4a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  unaff_A6[0x14] = 0xe60b2;
  *unaff_A6 = 0x98e62;
  uVar3 = false;
  candidate_99050();
  if ((bool)uVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  candidate_4c24();
  if (((bool)uVar3) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(char *)((int)unaff_A6 + 0x5b) + 1U & 7) < 3)) {
    *(byte *)(unaff_A6[3] + 0x8c) = *(byte *)(unaff_A6[3] + 0x8c) | 1;
  }
  return;
}
