/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04cab4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4cab4(void)

{
  undefined1 uVar1;
  undefined4 uVar2;
  undefined4 unaff_D3;
  undefined4 *unaff_A6;
  undefined8 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  uVar2 = candidate_4ce5c();
  if (*(char *)(unaff_A6[3] + 0xa4) == '\0') {
    candidate_260c8(uVar2);
  }
  else {
    candidate_260c8(uVar2);
  }
  uVar2 = candidate_4e5d0();
  uVar1 = *(undefined1 *)((int)unaff_A6 + 0xa2);
  uVar3 = candidate_3b36e();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar3;
  *(undefined4 *)((int)unaff_A6 + 0x76) = uVar2;
  *(uint *)((int)unaff_A6 + 0x7a) = CONCAT31((int3)((uint)unaff_D3 >> 8),uVar1) & 0xffff00ff;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *unaff_A6 = candidate_4cb74;
  return;
}
