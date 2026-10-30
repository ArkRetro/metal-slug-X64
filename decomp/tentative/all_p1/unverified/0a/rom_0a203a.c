/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a203a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a203a(void)

{
  ushort uVar1;
  char cVar2;
  undefined4 uVar3;
  undefined4 unaff_D7;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_146e8();
  candidate_1b1dc();
  uVar1 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x6e) = uVar1 & 0xf;
  cVar2 = candidate_29c6();
  uVar3 = 0x10000;
  if (cVar2 < '\0') {
    uVar3 = 0xffff0000;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar3;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  candidate_9ea8();
  candidate_4256();
  candidate_1abc8();
  *unaff_A6 = 0xa20ac;
  uVar4 = *(ushort *)(unaff_A6 + 0xb) < *(ushort *)((int)unaff_A6 + 0x6e);
  if ((short)*(ushort *)((int)unaff_A6 + 0x6e) < (short)*(ushort *)(unaff_A6 + 0xb)) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_143dc();
    }
    else {
      candidate_1456a();
      uVar1 = (ushort)((uint)unaff_D7 >> 0x10);
      if (uVar1 == 5) {
        *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6 + 0xb);
      }
      uVar4 = uVar1 < 5;
    }
  }
  else {
    unaff_A6[0xd] = 0;
    unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
    candidate_4256();
    *unaff_A6 = 0xa2106;
    uVar4 = 0;
    candidate_1abc8();
    candidate_1ac38();
  }
  candidate_1ad8c();
  candidate_427a();
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
