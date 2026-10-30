/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d512e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d512e(void)

{
  undefined1 uVar5;
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  ushort uVar4;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x83) != '\0') {
    if (*(char *)(unaff_A6 + 0x83) == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x7f) = 3;
      *(undefined1 *)(unaff_A6 + 0x80) = 3;
      *(undefined1 *)(unaff_A6 + 0x81) = 0;
      *(undefined1 *)(unaff_A6 + 0x82) = 0;
      *(undefined1 *)(unaff_A6 + 0x7e) = 0;
      if ((*(byte *)(unaff_A6 + 0x84) & 1) != 0) {
        *(undefined1 *)(unaff_A6 + 0x7f) = 0;
        *(undefined1 *)(unaff_A6 + 0x80) = 0;
        *(undefined1 *)(unaff_A6 + 0x81) = 3;
        *(undefined1 *)(unaff_A6 + 0x82) = 0;
        *(undefined1 *)(unaff_A6 + 0x7e) = 0;
      }
      sVar2 = candidate_260c8();
      sVar1 = sVar2 + 0x1e;
      if (*(char *)(unaff_A6 + 0x60) != '\x01') {
        sVar1 = sVar2 + 0x10;
      }
      *(short *)(unaff_A6 + 0x76) = sVar1;
      *(char *)(unaff_A6 + 0x84) = *(char *)(unaff_A6 + 0x84) + -1;
      if (*(char *)(unaff_A6 + 0x84) < '\0') {
        *(undefined1 *)(unaff_A6 + 0x83) = 2;
      }
      return;
    }
    *(undefined1 *)(unaff_A6 + 0x83) = 0;
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x76) = uVar3;
    uVar4 = candidate_29c6();
    *(short *)(unaff_A6 + 0x76) = (uVar4 & 0x3f) + *(short *)(unaff_A6 + 0x76);
    *(undefined1 *)(unaff_A6 + 0x7f) = 4;
    *(undefined1 *)(unaff_A6 + 0x80) = 4;
    *(undefined1 *)(unaff_A6 + 0x81) = 4;
    *(undefined1 *)(unaff_A6 + 0x82) = 0;
    *(undefined1 *)(unaff_A6 + 0x7e) = 0;
    return;
  }
  if (*(char *)(unaff_A6 + 0x60) != '\x01') {
    uVar5 = candidate_260c8();
    *(undefined1 *)(unaff_A6 + 0x84) = uVar5;
    sVar1 = candidate_260c8();
    *(short *)(unaff_A6 + 0x76) = sVar1 + 0x1e;
    *(undefined1 *)(unaff_A6 + 0x83) = 1;
    *(undefined1 *)(unaff_A6 + 0x7f) = 2;
    *(undefined1 *)(unaff_A6 + 0x80) = 2;
    *(undefined1 *)(unaff_A6 + 0x81) = 2;
    *(undefined1 *)(unaff_A6 + 0x82) = 0;
    *(undefined1 *)(unaff_A6 + 0x7e) = 2;
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x84) = 0x7f;
  sVar1 = candidate_260c8();
  *(short *)(unaff_A6 + 0x76) = sVar1 + 0x10;
  *(undefined1 *)(unaff_A6 + 0x83) = 1;
  *(undefined1 *)(unaff_A6 + 0x7f) = 2;
  *(undefined1 *)(unaff_A6 + 0x80) = 2;
  *(undefined1 *)(unaff_A6 + 0x81) = 0;
  *(undefined1 *)(unaff_A6 + 0x82) = 0;
  *(undefined1 *)(unaff_A6 + 0x7e) = 2;
  return;
}
