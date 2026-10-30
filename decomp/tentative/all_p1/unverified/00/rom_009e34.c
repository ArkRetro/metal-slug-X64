/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009e34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e34(void)

{
  short sVar1;
  short sVar2;
  undefined2 in_D0w;
  int in_A1;
  short *unaff_A2;

  sVar1 = *unaff_A2;
  sVar2 = unaff_A2[1];
  *(short *)(in_A1 + sVar2) = sVar1;
  *(short *)(in_A1 + 2 + (int)sVar1) = sVar2;
  sVar1 = *(short *)(in_A1 + 0x1000);
  unaff_A2[1] = 0x1000;
  *unaff_A2 = sVar1;
  *(undefined2 *)(in_A1 + 2 + (int)sVar1) = in_D0w;
  *(undefined2 *)(in_A1 + 0x1000) = in_D0w;
  unaff_A2[5] = -1;
  unaff_A2[6] = -1;
  *(undefined1 *)((int)unaff_A2 + 0xf) = 2;
  return;
}
