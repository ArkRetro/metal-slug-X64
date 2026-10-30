/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078474.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78474(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 2;
  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  *(byte *)(unaff_A6[3] + 0x9c) =
       *(byte *)(unaff_A6[3] + 0x9c) | '\x01' << (*(ushort *)((int)unaff_A6 + 0x8a) & 7);
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_4256();
  *unaff_A6 = 0x784e4;
  candidate_1abc8();
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x9b) != '\0') {
    *unaff_A6 = candidate_1c6ee;
  }
  uVar1 = false;
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_78540;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_78d54();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
    uVar1 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_1c6ee;
  }
  return;
}
