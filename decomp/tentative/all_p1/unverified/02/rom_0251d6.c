/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0251d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_251d6(void)

{
  int *piVar1;
  int unaff_A6;
  undefined8 uVar2;

  piVar1 = (int *)((uint)*(ushort *)(unaff_A6 + 0x88) * 0x10 + *(int *)(unaff_A6 + 0x84));
  if (*piVar1 == -1) {
    return 0;
  }
  *(undefined2 *)(unaff_A6 + 0x94) = *(undefined2 *)(piVar1 + 1);
  *(undefined2 *)(unaff_A6 + 0x9a) = *(undefined2 *)((int)piVar1 + 6);
  candidate_36a4(0xc714e,piVar1);
  uVar2 = candidate_2802();
  *(short *)(unaff_A6 + 0x88) = *(short *)(unaff_A6 + 0x88) + 1;
  return uVar2;
}
