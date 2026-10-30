/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06905a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6905a(void)

{
  undefined2 uVar1;
  short sVar2;
  short sVar3;
  short extraout_D1w;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  candidate_68f98();
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x4e) = uVar1;
  sVar2 = candidate_260c8();
  *(int *)(unaff_A6 + 0x8a) = (int)sVar2 << 8;
  *(undefined4 *)(unaff_A6 + 0x62) = 0;
  *(undefined1 *)(unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x60) = 0;
  *(undefined1 *)(unaff_A6 + 0x81) = 0;
  *(undefined1 *)(unaff_A6 + 0x9b) = 0;
  *(undefined1 *)(unaff_A6 + 0x9a) = 0;
  sVar3 = candidate_29f2();
  sVar2 = extraout_D1w;
  if (*(char *)(unaff_A6 + 0x9c) == '\0') {
    sVar2 = 0x38;
  }
  if (*(char *)(unaff_A6 + 0x9c) == '\x04') {
    sVar2 = 0x58;
  }
  if (*(char *)(unaff_A6 + 0x9c) == '\b') {
    sVar2 = 0x58;
  }
  *(short *)(unaff_A6 + 0x94) = sVar2 - sVar3;
  *(undefined2 *)(unaff_A6 + 0x8e) = *(undefined2 *)(unaff_A6 + 0x28);
  candidate_690ec();
  return;
}
