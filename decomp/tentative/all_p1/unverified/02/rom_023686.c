/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023686.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23686(void)

{
  byte bVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(unaff_A6[3] + 0x72);
  *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(unaff_A6[3] + 0x8a);
  if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar2;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  }
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x01') {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar2;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  }
  if (*(short *)(unaff_A6 + 0x26) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    candidate_40bc();
    *unaff_A6 = 0x23726;
    uVar3 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x8d) & 1) != 0) {
      *unaff_A6 = candidate_237a0;
      uVar3 = 0;
    }
    candidate_4c24();
    if ((bool)uVar3) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar3 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_23e96();
      *(byte *)(unaff_A6[3] + 0x8f) =
           *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
      *unaff_A6 = candidate_24164;
      uVar3 = 0;
    }
    candidate_24c94();
    candidate_24a94();
    if ((bool)uVar3) {
      candidate_23e96();
      *(byte *)(unaff_A6[3] + 0x8f) =
           *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
      *unaff_A6 = candidate_240ee;
    }
    return;
  }
  candidate_23e96();
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  candidate_4256();
  *unaff_A6 = candidate_235e8;
  if ((*(byte *)(unaff_A6[3] + 0x8d) & 8) == 0) {
    unaff_A6[0x14] = 0xe008c;
  }
  else {
    unaff_A6[0x14] = 0;
  }
  candidate_1abc8();
  candidate_427a();
  uVar3 = false;
  if ((*(char *)(unaff_A6[3] + 0x8e) != '\0') &&
     (bVar1 = *(byte *)(unaff_A6[3] + 0x8b), uVar3 = *(byte *)((int)unaff_A6 + 0x8b) < bVar1,
     *(byte *)((int)unaff_A6 + 0x8b) == bVar1)) {
    *unaff_A6 = candidate_23686;
    uVar3 = false;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
    uVar3 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar3) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_240ee;
  }
  return;
}
