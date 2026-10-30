/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x099090.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_99090(void)

{
  uint uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_19240();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  if (-1 < *(short *)(unaff_A6 + 0x1c)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  uVar2 = *(ushort *)((int)unaff_A6 + 0x72);
  if ((short)uVar2 < 0) {
    uVar2 = candidate_29c6();
  }
  *(ushort *)((int)unaff_A6 + 0x72) = uVar2 & 1;
  if (*(short *)((int)unaff_A6 + 0x76) == 0) {
    candidate_4256();
    *unaff_A6 = 0x990f2;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = 0x9910c;
      uVar3 = 0;
    }
  }
  else {
    candidate_9954a();
    candidate_4256();
    *unaff_A6 = 0x99130;
    if ((*(short *)(unaff_A6 + 0x1d) != 0) && (*(short *)(unaff_A6 + 0xd) < 0)) {
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
      *unaff_A6 = 0x99152;
    }
    uVar1 = unaff_A6[0xd];
    uVar3 = CARRY4(unaff_A6[0x1e],uVar1);
    unaff_A6[0xd] = unaff_A6[0x1e] + uVar1;
    func_0x000143e4();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_9917a;
      uVar3 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x40) == 0) {
    candidate_4c24();
    if ((bool)uVar3) {
      candidate_406b6();
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_992e4;
  }
  return;
}
