/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072930.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72930(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  candidate_1b1dc();
  candidate_72d52();
  *unaff_A6 = 0x72950;
  candidate_1436a();
  candidate_72bfc();
  uVar2 = CARRY2((ushort)*(byte *)(unaff_A6 + 0x1e) * 2,(ushort)*(byte *)(unaff_A6 + 0x1e) * 2);
  uVar1 = candidate_aa610();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
    uVar2 = false;
    uVar1 = candidate_aa760();
    if ((bool)uVar2) {
      *(undefined2 *)(unaff_A6 + 0x1c) = uVar1;
      candidate_aa83a();
      *unaff_A6 = candidate_729ac;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_728a2;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
