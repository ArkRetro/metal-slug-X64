/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5e7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5e7c(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  candidate_1b1dc();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_1ad6e();
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
  if (*(char *)(unaff_A6[3] + 0x96) != '\x03') {
    candidate_40bc();
    candidate_40bc();
    candidate_40bc();
  }
  candidate_4256();
  *unaff_A6 = 0xc5f8a;
  if (*(char *)(unaff_A6[3] + 0x97) == '\x01') {
    *unaff_A6 = candidate_c5fea;
  }
  candidate_44f6();
  candidate_1abc8();
  *(ushort *)((int)unaff_A6 + 0x1e) =
       CONCAT11((char)((ushort)*(undefined2 *)(unaff_A6[3] + 0x1e) >> 8),
                (char)*(undefined2 *)(unaff_A6[3] + 0x1e) - *(char *)((int)unaff_A6 + 0x96));
  candidate_427a();
  iVar1 = unaff_A6[3];
  if (((*(char *)(iVar1 + 0x97) == '\x06') || (*(char *)(iVar1 + 0x97) == '\x05')) ||
     (*(char *)(iVar1 + 0x97) == '\a')) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
