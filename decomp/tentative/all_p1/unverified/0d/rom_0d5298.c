/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5298.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d5298(void)

{
  undefined1 uVar4;
  short sVar1;
  undefined2 uVar2;
  ushort uVar3;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x85) == '\0') {
    uVar4 = candidate_260c8();
    *(undefined1 *)(unaff_A6 + 0x86) = uVar4;
    sVar1 = candidate_260c8();
    *(short *)(unaff_A6 + 0x78) = sVar1 + 0x1e;
    *(undefined1 *)(unaff_A6 + 0x85) = 1;
    *(undefined1 *)(unaff_A6 + 0x7f) = 0;
    *(undefined1 *)(unaff_A6 + 0x80) = 0;
    *(undefined1 *)(unaff_A6 + 0x81) = 0;
    *(undefined1 *)(unaff_A6 + 0x82) = 2;
    *(undefined1 *)(unaff_A6 + 0x7e) = 3;
    return;
  }
  if (*(char *)(unaff_A6 + 0x85) == '\x01') {
    *(undefined1 *)(unaff_A6 + 0x7f) = 0;
    *(undefined1 *)(unaff_A6 + 0x80) = 0;
    *(undefined1 *)(unaff_A6 + 0x81) = 0;
    *(undefined1 *)(unaff_A6 + 0x82) = 3;
    *(undefined1 *)(unaff_A6 + 0x7e) = 0;
    sVar1 = candidate_260c8();
    *(short *)(unaff_A6 + 0x78) = sVar1 + 0x1e;
    *(undefined1 *)(unaff_A6 + 0x85) = 2;
    return;
  }
  sVar1 = candidate_260c8();
  *(short *)(unaff_A6 + 0x78) = sVar1 + 0x1e;
  *(char *)(unaff_A6 + 0x86) = *(char *)(unaff_A6 + 0x86) + -1;
  if (*(char *)(unaff_A6 + 0x86) < '\0') {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x78) = uVar2;
    uVar3 = candidate_29c6();
    *(short *)(unaff_A6 + 0x78) = (uVar3 & 0x3f) + *(short *)(unaff_A6 + 0x78);
  }
  *(undefined1 *)(unaff_A6 + 0x85) = 0;
  *(undefined1 *)(unaff_A6 + 0x7f) = 0;
  *(undefined1 *)(unaff_A6 + 0x80) = 0;
  *(undefined1 *)(unaff_A6 + 0x81) = 0;
  *(undefined1 *)(unaff_A6 + 0x82) = 4;
  *(undefined1 *)(unaff_A6 + 0x7e) = 0;
  return;
}
