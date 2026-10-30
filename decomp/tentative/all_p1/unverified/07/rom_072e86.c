/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072e86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72e86(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_74c00();
  candidate_73cd8();
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  candidate_74b10();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xfe;
  candidate_353a4();
  candidate_1b1dc();
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0x72ed6;
  uVar2 = 0;
  candidate_74b74();
  candidate_74c3c();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x73ab0;
    uVar2 = 0;
  }
  uVar1 = candidate_aa610();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
    bVar3 = false;
    uVar1 = candidate_aa760();
    if (bVar3) {
      *(undefined2 *)(unaff_A6 + 0x20) = uVar1;
      candidate_aa83a();
      *unaff_A6 = candidate_72f3e;
    }
  }
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xbf;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
  if (*(char *)(unaff_A6 + 0x27) != '\0') {
    *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
  }
  return;
}
