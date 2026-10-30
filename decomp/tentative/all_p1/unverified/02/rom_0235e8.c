/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0235e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_235e8(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  if ((*(byte *)(unaff_A6[3] + 0x8d) & 8) == 0) {
    unaff_A6[0x14] = 0xe008c;
  }
  else {
    unaff_A6[0x14] = 0;
  }
  candidate_1abc8();
  candidate_427a();
  uVar2 = false;
  if ((*(char *)(unaff_A6[3] + 0x8e) != '\0') &&
     (bVar1 = *(byte *)(unaff_A6[3] + 0x8b), uVar2 = *(byte *)((int)unaff_A6 + 0x8b) < bVar1,
     *(byte *)((int)unaff_A6 + 0x8b) == bVar1)) {
    *unaff_A6 = candidate_23686;
    uVar2 = false;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
    uVar2 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar2) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_240ee;
  }
  return;
}
