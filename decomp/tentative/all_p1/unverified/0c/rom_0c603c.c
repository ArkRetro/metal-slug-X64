/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c603c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c603c(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_4256();
  *unaff_A6 = 0xc5f9e;
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
