/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08fa1a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8fa1a(void)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 in_D0b;
  undefined1 in_D1b;
  int unaff_A6;
  undefined1 uStack_9;

  uVar1 = *(undefined1 *)(unaff_A6 + 0x8a);
  uVar2 = *(undefined1 *)(unaff_A6 + 0x88);
  *(undefined1 *)(unaff_A6 + 0x8a) = in_D0b;
  *(undefined1 *)(unaff_A6 + 0x88) = in_D1b;
  *(byte *)(unaff_A6 + 0x94) = *(byte *)(unaff_A6 + 0x94) | 1;
  candidate_8fa4e();
  *(undefined1 *)(unaff_A6 + 0x8a) = uVar1;
  *(undefined1 *)(unaff_A6 + 0x88) = uStack_9;
  *(undefined1 *)(unaff_A6 + 0x94) = uVar2;
  return;
}
